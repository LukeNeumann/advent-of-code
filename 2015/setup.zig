const std = @import("std");

pub fn setup_day(day_string: []u8) !void {
    std.fs.cwd().makeDir(day_string) catch |err| {
        if (err == error.PathAlreadyExists) {} else return err;
    };
    var day_dir = try std.fs.cwd().openDir(day_string, .{});
    defer day_dir.close();

    var new_input_file = false;
    var input_file = day_dir.openFile("input.txt", .{}) catch |err| blk: {
        if (err == error.FileNotFound) {
            new_input_file = true;
            break :blk try day_dir.createFile("input.txt", .{});
        } else return err;
    };
    defer input_file.close();
    if (new_input_file) {
        std.debug.print("touching new input.txt in {s}\n", .{day_string});
    }

    var new_root_file = false;
    var root_file = day_dir.openFile("root.zig", .{}) catch |err| blk: {
        if (err == error.FileNotFound) {
            new_root_file = true;
            break :blk try day_dir.createFile("root.zig", .{});
        } else return err;
    };
    defer root_file.close();
    if (new_root_file) {
        std.debug.print("writing new root.zig in {s}\n", .{day_string});
        try root_file.writeAll(
            \\const std = @import("std");
            \\const input = @embedFile("input.txt");
            \\
            \\pub fn solve() !void {
            \\    var gpa: std.heap.GeneralPurposeAllocator(.{}) = .init;
            \\    const alloc = gpa.allocator();
            \\    defer _ = gpa.deinit(); 
            \\    _ = alloc;
            \\
            \\    var it = std.mem.tokenizeScalar(u8, input, '\n');
            \\    while (it.next()) |line| {
            \\        _ = line;
            \\    }
            \\    std.debug.print("{d}\n{d}\n", .{ 0, 0 });
            \\}
        );
    }
}

pub fn get_inputs() !void {}

pub fn main() !void {
    var general_purpose_allocator: std.heap.GeneralPurposeAllocator(.{}) = .init;
    const gpa = general_purpose_allocator.allocator();

    for (1..26) |day| {
        const day_str = try std.fmt.allocPrint(gpa, "day{d:0>2}", .{day});
        try setup_day(day_str);
    }

    try get_inputs();
}
