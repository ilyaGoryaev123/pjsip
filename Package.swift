// swift-tools-version: 5.8
// The swift-tools-version declares the minimum version of Swift required to build this package.

import PackageDescription

let package = Package(
    name: "PJProjectSPM",
	platforms: [
		.iOS(.v13),
		.macOS(.v11)
	],
    products: [
        // Products define the executables and libraries a package produces, making them visible to other packages.
        .library(
            name: "PJProject",
            targets: ["PJProject"]),
    ],
	targets: [
		.binaryTarget(
			name: "PJProjectBinary",
			path: "Sources/PJProjectSPM/libpjproject.xcframework"
		),
		.target(
			name: "PJProject",
			dependencies: ["PJProjectBinary"],
			path: "Sources/PJProjectSPM",
			publicHeadersPath: "include",
			cSettings: [
				.headerSearchPath("include")
			]
		)
	]
)
