fvScalarMatrix passiveEqn
(
    fvm::ddt(passive) + fvm::div(phi, passive) - fvm::laplacian(D, passive)
);
