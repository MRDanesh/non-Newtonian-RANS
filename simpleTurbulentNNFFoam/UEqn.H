    // Momentum predictor

    // non-Newtonian stress

	const volScalarField& omega = mesh.lookupObject<volScalarField>("omega");
	const volScalarField& k = mesh.lookupObject<volScalarField>("k");
    const volScalarField& nu = mesh.lookupObject<volScalarField>("nu");

    tmp<volTensorField> tgradU = fvc::grad(U);
    volScalarField extraNN = C_beta*betaStar_*omega*k;  //extra term for non-Newtonian
    volScalarField S2 = 2*magSqr(symm(tgradU()));       // Non-modified mean strain rate (squared)

    volScalarField newS2 = S2;  // initilization
    volScalarField gammaDot = sqrt(newS2);  // initilization
    dimensionedScalar SMALL_gammaDot("SMALL_gamma", gammaDot.dimensions(), VSMALL);
    gammaDot = max(gammaDot, SMALL_gammaDot); // stabilization

    //nu_modified = tau0 / gammaDot + K * pow(gammaDot, n - 1.0); // initilization

    // Step 3: iterative update
    for (int iter = 0; iter < 5; ++iter)
    {
        volScalarField newS2 = S2 + extraNN/nu_modified;
        gammaDot = sqrt(newS2);
        gammaDot = max(gammaDot, SMALL_gammaDot); // stabilization
        nu_modified = tau0 / gammaDot + K * pow(gammaDot, n - 1.0);

        // Compute and write the sum of absolute differences
        scalar sumError = sum(mag(nu_modified - nu)).value();
        Info << "Iteration " << iter << ": sum of error = " << sumError << nl << endl;
    }    
    
    volScalarField dmu_dgamma = -tau0 / (gammaDot * gammaDot) + K * (n - 1.0) * pow(gammaDot, n - 2.0);
    volScalarField muNN = dmu_dgamma * extraNN/ (nu_modified*gammaDot); 


    MRF.correctBoundaryVelocity(U);

    tmp<fvVectorMatrix> tUEqn
    (
        fvm::div(phi, U)
      + MRF.DDt(U)
      + g
      + turbulence->divDevReff(U)
      - fvc::div((muNN+nu_modified-nu) * dev2(2*symm(fvc::grad(U))-fvc::grad(U))) - fvm::laplacian(muNN+nu_modified-nu, U)
     ==
        fvOptions(U)
    );
    fvVectorMatrix& UEqn = tUEqn.ref();

    tgradU.clear();
    UEqn.relax();

    fvOptions.constrain(UEqn);

    if (simple.momentumPredictor())
    {
        solve(UEqn == -fvc::grad(p));

        fvOptions.correct(U);
    }
