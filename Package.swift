// swift-tools-version:5.1
// The swift-tools-version declares the minimum version of Swift required to build this package.

import PackageDescription

let package = Package(
    name: "HollowCore",
    products: [
        // Products define the executables and libraries produced by a package, and make them visible to other packages.
        .library(
            name: "HollowCore",
            targets: [
                "HollowCoreCore",
                "HollowCoreData",
                "HollowCoreContainer",
                "HollowCoreGeometry",
                "HollowCoreGraphic",
                "HollowCoreJSON",
                "HollowCoreThread",
        ]),
    ],
    dependencies: [],
    targets: [
        // Targets are the basic building blocks of a package. A target can define a module or a test suite.
        // Targets can depend on other targets in this package, and on products in packages which this package depends on.
        .target(
            name: "HollowCoreCore",
            path: "./Source/Core",
            publicHeadersPath: "."),
        .target(
            name: "HollowCoreData",
            path: "./Source/Data",
            publicHeadersPath: "."),
        .target(
            name: "HollowCoreContainer",
            path: "./Source/Container",
            publicHeadersPath: "."),
        .target(
            name: "HollowCoreGeometry",
            path: "./Source/Geometry",
            publicHeadersPath: "."),
        .target(
            name: "HollowCoreGraphic",
            path: "./Source/Graphic",
            publicHeadersPath: "."),
        .target(
            name: "HollowCoreJSON",
            path: "./Source/JSON",
            publicHeadersPath: "."),
        .target(
            name: "HollowCoreThread",
            path: "./Source/Thread",
            publicHeadersPath: "."),
    ]
)
