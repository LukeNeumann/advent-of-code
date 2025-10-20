//! By convention, root.zig is the root source file when making a library.
const std = @import("std");
const input = @embedFile("input.txt");

pub fn solve() !void {
    var it = std.mem.tokenizeScalar(u8, input, '\n');
    var total_paper: u64 = 0;
    var total_ribbon: u64 = 0;
    while (it.next()) |line| {
        var dimensions: [3]u32 = undefined;
        var it2 = std.mem.tokenizeScalar(u8, line, 'x');
        for (0..dimensions.len) |i| {
            dimensions[i] = try std.fmt.parseInt(u32, it2.next().?, 10);
        }

        var side_areas: [3]u32 = undefined;
        var side_perimeters: [3]u32 = undefined;
        for (0..dimensions.len) |i| {
            side_areas[i] = dimensions[i] * dimensions[(i + 1) % 3];
            side_perimeters[i] = dimensions[i] * 2 + dimensions[(i + 1) % 3] * 2;
        }

        const total_bow = @min(side_perimeters[0], side_perimeters[1], side_perimeters[2]) + (dimensions[0] * dimensions[1] * dimensions[2]);
        const total_area = side_areas[0] * 2 + side_areas[1] * 2 + side_areas[2] * 2 + @min(side_areas[0], side_areas[1], side_areas[2]);
        total_paper += total_area;
        total_ribbon += total_bow;
    }
    std.debug.print("{d}\n{d}\n", .{ total_paper, total_ribbon });
}
