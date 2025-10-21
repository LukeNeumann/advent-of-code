const std = @import("std");
const input = @embedFile("input.txt");

pub fn solve_part(alloc: std.mem.Allocator, part: u32) !i64 {
    const Position = struct { x: i32, y: i32 };
    var curr_pos: Position = .{ .x = 0, .y = 0 };
    var robo_pos: Position = .{ .x = 0, .y = 0 };

    var visited = std.AutoHashMap(Position, u32).init(alloc);
    defer visited.deinit();
    try visited.put(curr_pos, 1);

    var it = std.mem.tokenizeScalar(u8, input, '\n');
    while (it.next()) |line| {
        for (0..line.len, line) |i, c| {
            var santa_sel: *Position = &curr_pos;
            if (part == 2 and i % 2 == 1) {
                santa_sel = &robo_pos;
            }
            switch (c) {
                '^' => santa_sel.y += 1,
                'v' => santa_sel.y -= 1,
                '>' => santa_sel.x += 1,
                '<' => santa_sel.x -= 1,
                else => {},
            }

            if (visited.contains(santa_sel.*)) {
                visited.getPtr(santa_sel.*).?.* += 1;
            } else {
                try visited.put(santa_sel.*, 1);
            }
        }
    }
    return visited.count();
}

pub fn solve() !void {
    var gpa: std.heap.GeneralPurposeAllocator(.{}) = .init;
    const alloc = gpa.allocator();
    defer _ = gpa.deinit();

    std.debug.print("{d}\n{d}\n", .{ try solve_part(alloc, 1), try solve_part(alloc, 2) });
}
