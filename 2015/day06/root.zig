const std = @import("std");
const input = @embedFile("input.txt");

const Command = struct {
    op: enum {
        Toggle,
        TurnOn,
        TurnOff,
    } = .Toggle,
    startx: u32 = 0,
    starty: u32 = 0,
    endx: u32 = 0,
    endy: u32 = 0,
};

pub fn parse_xy(in: []const u8) !struct { u32, u32 } {
    //std.debug.print("parse_xy: {s}\n", .{in});
    var it = std.mem.tokenizeScalar(u8, in, ',');
    const x = try std.fmt.parseInt(u32, it.next().?, 10);
    const y = try std.fmt.parseInt(u32, it.next().?, 10);
    return .{ x, y };
}

pub fn solve() !void {
    var gpa: std.heap.GeneralPurposeAllocator(.{}) = .init;
    const alloc = gpa.allocator();
    defer _ = gpa.deinit();

    var it = std.mem.tokenizeScalar(u8, input, '\n');
    var cmds = try std.ArrayList(Command).initCapacity(alloc, 300);
    defer cmds.deinit(alloc);
    while (it.next()) |line| {
        var cmd_it = std.mem.tokenizeScalar(u8, line, ' ');
        var cmd: Command = .{};

        const turn_toggle = cmd_it.next().?;
        if (std.mem.eql(u8, turn_toggle, "turn")) {
            const on_off = cmd_it.next().?;
            if (std.mem.eql(u8, on_off, "on")) {
                cmd.op = .TurnOn;
            } else if (std.mem.eql(u8, on_off, "off")) {
                cmd.op = .TurnOff;
            }
        } else if (std.mem.eql(u8, turn_toggle, "toggle")) {
            cmd.op = .Toggle;
        }

        cmd.startx, cmd.starty = try parse_xy(cmd_it.next().?);

        _ = cmd_it.next(); // "through"

        cmd.endx, cmd.endy = try parse_xy(cmd_it.next().?);

        try cmds.append(alloc, cmd);
    }

    //std.debug.print("done parsing\n", .{});
    var grid: [1000][1000]bool = @splat(@splat(false));
    var grid2: [1000][1000]u16 = @splat(@splat(0));

    for (cmds.items, 0..cmds.items.len) |cmd, _| {
        for (cmd.startx..(cmd.endx + 1)) |x| {
            for (cmd.starty..(cmd.endy + 1)) |y| {
                //std.debug.print("\r{d},{d}", .{ x, y });
                switch (cmd.op) {
                    .TurnOn => grid[x][y] = true,
                    .TurnOff => grid[x][y] = false,
                    .Toggle => grid[x][y] = !grid[x][y],
                }
                switch (cmd.op) {
                    .TurnOn => grid2[x][y] += 1,
                    .TurnOff => {
                        if (grid2[x][y] > 0) grid2[x][y] -= 1;
                    },
                    .Toggle => grid2[x][y] += 2,
                }
            }
        }
        //std.debug.print("\n{d}: {d},{d} -> {d},{d}\n", .{ i + 1, cmd.startx, cmd.starty, cmd.endx, cmd.endy });
    }

    //std.debug.print("done executing", .{});

    var count: u32 = 0;
    for (grid) |row| {
        for (row) |item| count += if (item) 1 else 0;
    }
    var count2: u64 = 0;
    for (grid2) |row| {
        for (row) |item| count2 += item;
    }

    std.debug.print("{d}\n{d}\n", .{ count, count2 });
}
