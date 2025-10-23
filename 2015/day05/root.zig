const std = @import("std");
const input = @embedFile("input.txt");

pub fn solve() !void {
    var gpa: std.heap.GeneralPurposeAllocator(.{}) = .init;
    const alloc = gpa.allocator();
    defer _ = gpa.deinit();

    var nice_count_part1: u32 = 0;
    var nice_count_part2: u32 = 0;
    const vowels = "aeiou";

    var it = std.mem.tokenizeScalar(u8, input, '\n');
    while (it.next()) |line| {
        var vowel_count: u32 = 0;
        var double_count: u32 = 0;
        var is_bad = false;
        var pairs: std.BufMap = .init(alloc);
        defer pairs.deinit();
        var next_pair_key: ?[2]u8 = null;
        var has_double_pair = false;
        var sandwich_count: u32 = 0;
        for (0..line.len, line) |i, c| {
            if (std.mem.containsAtLeast(u8, vowels, 1, &[1]u8{c})) vowel_count += 1;
            if (i > 0) {
                const prev_c = line[i - 1];
                if (c == prev_c) double_count += 1;

                const curr_slice: [2]u8 = .{ prev_c, c };
                if (std.mem.eql(u8, &curr_slice, "ab") or
                    std.mem.eql(u8, &curr_slice, "cd") or
                    std.mem.eql(u8, &curr_slice, "pq") or
                    std.mem.eql(u8, &curr_slice, "xy"))
                {
                    is_bad = true;
                }

                if (pairs.get(&curr_slice) != null) {
                    has_double_pair = true;
                }

                if (next_pair_key != null) try pairs.put(&next_pair_key.?, "");
                next_pair_key = curr_slice;
            }
            if (i > 1) {
                const other = line[i - 2];
                if (c == other) sandwich_count += 1;
            }
        }

        const is_nice_part1 = !is_bad and (vowel_count > 2) and (double_count > 0);
        if (is_nice_part1) nice_count_part1 += 1;

        const is_nice_part2 = (sandwich_count > 0) and has_double_pair;
        if (is_nice_part2) nice_count_part2 += 1;
    }
    std.debug.print("{d}\n{d}\n", .{ nice_count_part1, nice_count_part2 });
}
