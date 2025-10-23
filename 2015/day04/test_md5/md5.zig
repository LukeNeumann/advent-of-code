const std = @import("std");

pub fn main() !void {
    var out: [std.crypto.hash.Md5.digest_length]u8 = undefined;
    for (0..10_000_000) |_| {
        var md5 = std.crypto.hash.Md5.init(.{});
        md5.update("yzbqklnj");
        md5.final(&out);
        //const out_hex = std.fmt.bytesToHex(out, std.fmt.Case.lower);
        //std.debug.print("zig {s}", .{out_hex});
    }
}
