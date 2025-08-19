#include "createRDeltaT.H"


IOdictionary transportProperties
(
    IOobject
    (
        "transportProperties",						
        runTime.constant(),						
        mesh,
        IOobject::MUST_READ_IF_MODIFIED,
        IOobject::NO_WRITE
    )
);


Info<< "Reading field p\n" << endl;
volScalarField p
(
    IOobject
    (
        "p",
        runTime.timeName(),
        mesh,
        IOobject::MUST_READ,
        IOobject::AUTO_WRITE
    ),
    mesh
);

Info<< "Reading field U\n" << endl;
volVectorField U
(
    IOobject
    (
        "U",
        runTime.timeName(),
        mesh,
        IOobject::MUST_READ,
        IOobject::AUTO_WRITE
    ),
    mesh
);


// concentration Field
volScalarField passive
(
   IOobject
  (
       "passive",
       runTime.timeName(),
       mesh,
       IOobject::MUST_READ,
       IOobject::AUTO_WRITE
  ),
   mesh
);

// Diffusivity
dimensionedScalar D
(
    transportProperties.lookup("D")
);

// Write mesh volume size
volScalarField cv     
(             
IOobject 
       (                     
"cv",
runTime.timeName(),
mesh,
IOobject::NO_READ,                     
IOobject::AUTO_WRITE 
), 
mesh,
dimensionedScalar("zero",dimVolume,0.0) 
); 
cv.ref() = mesh.V();

// ********** Read non-Newtonian *************//

volScalarField nu_modified
(
    IOobject
    (
        "nu_modified",
        runTime.timeName(),  // Current time dir
        mesh,
        IOobject::NO_READ,   // <-- Important! Don't look in 0/ directory
        IOobject::AUTO_WRITE // <-- Saves it like U, p, etc.
    ),
    mesh,
    dimensionedScalar("nu0", dimensionSet(0, 2, -1, 0, 0), 1e-6)
);


IOdictionary rheoCoeffsDict
(
    IOobject
    (
        "rheologyProperties",
        runTime.constant(),
        mesh,
        IOobject::MUST_READ_IF_MODIFIED,
        IOobject::NO_WRITE
    )
);

    dimensionedScalar K(rheoCoeffsDict.lookup("K"));
    dimensionedScalar tau0(rheoCoeffsDict.lookup("tau0"));
    dimensionedScalar n(rheoCoeffsDict.lookup("n"));
    dimensionedScalar betaStar_(rheoCoeffsDict.lookup("betaStar_"));
    dimensionedScalar C_beta(rheoCoeffsDict.lookup("C_beta"));
    dimensionedScalar C_tau(rheoCoeffsDict.lookup("C_tau"));

// Gravity 

#include "readGravitationalAcceleration.H"

#include "createPhi.H"


label pRefCell = 0;
scalar pRefValue = 0.0;
setRefCell(p, pimple.dict(), pRefCell, pRefValue);
mesh.setFluxRequired(p.name());


singlePhaseTransportModel laminarTransport(U, phi);

autoPtr<incompressible::turbulenceModel> turbulence
(
    incompressible::turbulenceModel::New(U, phi, laminarTransport)
);

#include "createMRF.H"
#include "createFvOptions.H"
