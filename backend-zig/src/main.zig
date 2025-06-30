const std = @import("std");
const httpz = @import("httpz");
const data = @import("data.zig");

comptime users: std.ArrayList(data.User),

pub fn main() !void {
    std.debug.print("All your {s} are belong to us.\n", .{"codebase"});
    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    const allocator = gpa.allocator();
    var server = try httpz.Server(void).init(allocator, .{ .port = 8000 }, {});
    users = std.ArrayList(data.User).init(allocator);
    defer {
        server.stop();
        server.deinit();
        users.deinit();
    }
    users.append(data.User.init(&("Jaipal").*), &("dffe86797a27a6cc1e7d4f3b7628783bc1292f310eeb352148f62a993c30c027").*);

    var router = try server.router(.{});
    router.get("/", check_msg, .{});
    router.get("/login", login, .{});

    std.debug.print("Server started at: http://localhost:8000/\n", .{});
    try server.listen();
}

fn check_msg(req: *httpz.Request, res: *httpz.Response) !void {
    if (req.body_buffer == null) {
        res.json(.{ .output = "No body found" }, .{});
    }
    res.status = 200;
    try res.json(.{ .author = "Jaipal", .msg = "Hello World", .timestamp = .{ .hr = 11, .min = 1 } }, .{});
}
fn login(req: *httpz.Request, res: *httpz.Response) !void {
    _ = req;
    res.status = 200;
    var name = ("Jaipal").*;
    try data.User.init(&name, &name).send_token(res);
}
