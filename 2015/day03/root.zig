const std = @import("std");
const input = @embedFile("input.txt");

pub fn solve() !void {
    var it = std.mem.tokenizeScalar(u8, input, '\n');
    while (it.next()) |line| {
        _ = line;
    }
    std.debug.print("{d}\n{d}\n", .{ 0, 0 });
}