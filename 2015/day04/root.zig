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
        while (true) : (i += 1) {
            var base_md5 = std.crypto.hash.Md5.init(.{});
            base_md5.update(line);

            const data = try std.fmt.allocPrint(alloc, "{d}", .{i});

            var md5 = base_md5;
            md5.update(data);
            md5.final(&out);
            const part2_match = std.mem.startsWith(u8, &out, &[3]u8{ 0, 0, 0 });
            const part1_match = std.mem.startsWith(u8, &out, &[2]u8{ 0, 0 }) and out[2] < 9;

            //const out_hex = std.fmt.bytesToHex(out, std.fmt.Case.lower);
            //std.debug.print("{s}", .{out_hex});

            if (part1 == null and part1_match) {
                part1 = i;
                std.debug.print("part1: {d}\n", .{part1.?});
            }
            if (part2 == null and part2_match) {
                part2 = i;
                std.debug.print("part2: {d}\n", .{part2.?});
            }

            if (part1 != null and part2 != null) break;
        }
    }
}
