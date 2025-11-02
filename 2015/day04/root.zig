const std = @import("std");
const input = @embedFile("input.txt");

pub fn solve() !void {
    var gpa: std.heap.GeneralPurposeAllocator(.{}) = .init;
    const alloc = gpa.allocator();
    var it = std.mem.tokenizeScalar(u8, input, '\n');
    var part1: ?u64 = null;
    var part2: ?u64 = null;
    var out: [std.crypto.hash.Md5.digest_length]u8 = undefined;

    var i: u64 = 0;
    while (it.next()) |line| {
        var data = try std.ArrayList(u8).initCapacity(alloc, line.len + 10);
        try data.appendSlice(alloc, line);
        try data.append(alloc, '0');
        while (true) : (i += 1) {
            //std.debug.print("\r{d}", .{i});
            //_ = c.MD5(data.items.ptr, data.items.len, &out);
            var md5 = std.crypto.hash.Md5.init(.{});
            md5.update(data.items);
            md5.final(&out);
            const part2_match = std.mem.startsWith(u8, &out, &[3]u8{ 0, 0, 0 });
            const part1_match = std.mem.startsWith(u8, &out, &[2]u8{ 0, 0 }) and out[2] < 9;

            if (part1 == null and part1_match) {
                part1 = i;
                std.debug.print("\rpart1: {d}\n", .{part1.?});
            }
            if (part2 == null and part2_match) {
                part2 = i;
                std.debug.print("\rpart2: {d}\n", .{part2.?});
            }

            if (part1 != null and part2 != null) break;

            data.items[data.items.len - 1] += 1;
            var carry_pos = data.items.len - 1;
            while (carry_pos >= line.len and data.items[carry_pos] == ('9' + 1)) {
                if (carry_pos == line.len) {
                    data.items[carry_pos] = '1';
                    try data.append(alloc, '0');
                    break;
                }
                data.items[carry_pos] = '0';
                data.items[carry_pos - 1] += 1;
                carry_pos -= 1;
            }
        }
    }
}
