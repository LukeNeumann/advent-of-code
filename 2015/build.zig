const std = @import("std");

// Although this function looks imperative, it does not perform the build
// directly and instead it mutates the build graph (`b`) that will be then
// executed by an external runner. The functions in `std.Build` implement a DSL
// for defining build steps and express dependencies between them, allowing the
// build runner to parallelize the build automatically (and the cache system to
// know when a step doesn't need to be re-run).
pub fn build(b: *std.Build) void {
    // Standard target options allow the person running `zig build` to choose
    // what target to build for. Here we do not override the defaults, which
    // means any target is allowed, and the default is native. Other options
    // for restricting supported target set are available.
    const target = b.standardTargetOptions(.{});
    // Standard optimization options allow the person running `zig build` to select
    // between Debug, ReleaseSafe, ReleaseFast, and ReleaseSmall. Here we do not
    // set a preferred release mode, allowing the user to decide how to optimize.
    const optimize = b.standardOptimizeOption(.{});
    // It's also possible to define more custom flags to toggle optional features
    // of this build script using `b.option()`. All defined flags (including
    // target and optimize options) will be listed when running `zig build --help`
    // in this directory.

    const setup_exe = b.addExecutable(.{
        .name = "setup",
        .root_module = b.createModule(.{ .root_source_file = b.path("setup.zig"), .target = target, .optimize = optimize }),
    });
    b.installArtifact(setup_exe);
    const setup_step = b.step("setup", "setup directories and files if they don't exist");
    const setup_cmd = b.addRunArtifact(setup_exe);
    setup_step.dependOn(&setup_cmd.step);

    const openssl = b.dependency("openssl", .{
        .target = target,
        .optimize = optimize,
    });
    const crypto_lib = openssl.artifact("crypto");
    const ssl_lib = openssl.artifact("ssl");

    for (1..26) |day| {
        const day_string = b.fmt("day{d:0>2}", .{day});
        const mod = b.addModule(day_string, .{
            .root_source_file = b.path(b.fmt("{s}/root.zig", .{day_string})),
            .target = target,
        });
        const exe = b.addExecutable(.{
            .name = b.fmt("solve_{s}", .{day_string}),
            .root_module = b.createModule(.{
                .root_source_file = b.path("main.zig"),
                .target = target,
                .optimize = optimize,
                .imports = &.{
                    .{ .name = "day", .module = mod },
                },
            }),
        });

        if (day == 4) {
            mod.linkLibrary(ssl_lib);
            mod.linkLibrary(crypto_lib);
            mod.addIncludePath(b.path("include"));
        }

        b.installArtifact(exe);
        const run_step = b.step(b.fmt("run_{s}", .{day_string}), "Run the day");
        const run_cmd = b.addRunArtifact(exe);
        run_step.dependOn(&run_cmd.step);
        run_cmd.step.dependOn(b.getInstallStep());
        run_cmd.step.dependOn(setup_step);

        if (b.args) |args| {
            run_cmd.addArgs(args);
        }

        const mod_tests = b.addTest(.{
            .root_module = mod,
        });

        const run_mod_tests = b.addRunArtifact(mod_tests);

        // Creates an executable that will run `test` blocks from the executable's
        // root module. Note that test executables only test one module at a time,
        // hence why we have to create two separate ones.
        const exe_tests = b.addTest(.{
            .root_module = exe.root_module,
        });

        // A run step that will run the second test executable.
        const run_exe_tests = b.addRunArtifact(exe_tests);

        // A top level step for running all tests. dependOn can be called multiple
        // times and since the two run steps do not depend on one another, this will
        // make the two of them run in parallel.
        const test_step = b.step(b.fmt("test_{s}", .{day_string}), "Run tests");
        test_step.dependOn(&run_mod_tests.step);
        test_step.dependOn(&run_exe_tests.step);
    }
}
