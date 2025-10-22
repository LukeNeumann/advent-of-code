const std = @import("std");

pub fn main() !void {
    for (0..1000000) |_| {
        var out: [std.crypto.hash.Md5.digest_length]u8 = undefined;
        var md5 = std.crypto.hash.Md5.init(.{});
        md5.update("abc");
        md5.final(&out);
        //const out_hex = std.fmt.bytesToHex(out, std.fmt.Case.lower);
        //std.debug.print("zig {s}", .{out_hex});
    }
}
