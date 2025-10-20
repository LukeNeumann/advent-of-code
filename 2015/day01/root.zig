//! By convention, root.zig is the root source file when making a library.
const std = @import("std");

pub fn solve() !void {
    const file = std.fs.cwd().openFile("day01/input.txt", .{ .mode = .read_only }) catch |err| {
        std.log.err("Failed to open input.txt {s}", .{@errorName(err)});
        return;
    };
    defer file.close();

    var read_buf: [2]u8 = undefined;
    var file_reader: std.fs.File.Reader = file.reader(&read_buf);
    const reader = &file_reader.interface;

    var floor : i64 = 0;
    var b_index : u32 = 0;
    var byte_count : u32 = 0;

    while(true) {
        const data = reader.takeByte() catch |err| {
            if(err == error.EndOfStream) break else return err;
        };
        byte_count += 1;
        if(data == '(') { floor += 1; }
        if(data == ')') { floor -= 1; }
        if(floor == -1 and b_index == 0) { b_index = byte_count; }
    }
    std.debug.print("{d}\n{d}\n", .{ floor, b_index });
}
