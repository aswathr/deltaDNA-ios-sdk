// swift-tools-version:5.6

import PackageDescription

let package = Package(
    name: "DeltaDNA",
    platforms: [
        .iOS(.v12),
    ],
    products: [
        .library(
            name: "DeltaDNA",
            targets: ["DeltaDNA"]),
    ],
    targets: [
        .binaryTarget(
            name: "DeltaDNA",
            path: "build/Products/XCFramework/DeltaDNA.xcframework"
        )
    ]
)
