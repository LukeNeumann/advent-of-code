const std = @import("std");
const c = @cImport({
    @cInclude("openssl/md5.h");
});
pub fn main() !void {
    for (0..1000000) |_| {
        var out: [std.crypto.hash.Md5.digest_length]u8 = undefined;
        _ = c.MD5("abc", 3, &out);
        //const out_hex = std.fmt.bytesToHex(out, std.fmt.Case.lower);
        //std.debug.print("c {s}", .{out_hex});
    }
}
