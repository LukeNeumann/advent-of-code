const std = @import("std");

pub fn build(b: *std.Build) void {
    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});

    const exe = b.addExecutable(.{
        .name = "zig_md5",
        .root_module = b.createModule(.{
            .root_source_file = b.path("md5.zig"),
            .target = target,
            .optimize = optimize,
            .imports = &.{},
        }),
    });
    b.installArtifact(exe);
    const run_step = b.step("run_zig", "");
    const run_cmd = b.addRunArtifact(exe);
    run_step.dependOn(&run_cmd.step);
    run_cmd.step.dependOn(b.getInstallStep());

    const openssl = b.dependency("openssl", .{
        .target = target,
        .optimize = optimize,
    });
    const crypto_lib = openssl.artifact("crypto");
    const ssl_lib = openssl.artifact("ssl");
    const exe2 = b.addExecutable(.{
        .name = "c_md5",
        .root_module = b.createModule(.{
            .root_source_file = b.path("c_md5.zig"),
            .target = target,
            .optimize = optimize,
            .imports = &.{},
        }),
    });
    exe2.linkLibrary(ssl_lib);
    exe2.linkLibrary(crypto_lib);
    exe2.addIncludePath(b.path("include"));
    b.installArtifact(exe2);
    const run_step2 = b.step("run_c", "");
    const run_cmd2 = b.addRunArtifact(exe2);
    run_step2.dependOn(&run_cmd2.step);
    run_cmd2.step.dependOn(b.getInstallStep());
}
