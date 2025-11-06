const std = @import("std");
const input = @embedFile("input.txt");

pub fn solve() !void {
    var gpa: std.heap.GeneralPurposeAllocator(.{}) = .init;
    const alloc = gpa.allocator();
    defer _ = gpa.deinit();

    var code_count: usize = 0;
    var parsed_count: usize = 0;
    var encoded_count: usize = 0;
    var it = std.mem.tokenizeScalar(u8, input, '\n');
    while (it.next()) |line| {
        const parsed = try std.zig.string_literal.parseAlloc(alloc, line);
        defer alloc.free(parsed);

        var writer = std.io.Writer.Allocating.init(alloc);
        defer writer.deinit();
        try std.zig.stringEscape(line, &writer.writer);
        encoded_count += writer.written().len + 2; // +2 for new quotes

        code_count += line.len;
        parsed_count += parsed.len;
        //std.debug.print("{s} ({d}) => {s} ({d}) => {s} ({d})\n", .{ line, line.len, parsed, parsed.len, writer.written(), writer.written().len });
    }
    std.debug.print("{d}\n{d}\n", .{ code_count - parsed_count, encoded_count - code_count });
}
