using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Remoting.Channels;
using System.Web;

namespace glfedge.Services {

    public static class ServoHelper {
        public static int SetPosition { get; set; }
        public static DateTime LastUpdate { get; set; }
    }

    public class ServoU : IHttpHandler {
        public void ProcessRequest(HttpContext context) {
            var x = context.Request.Params["i"];
            int i;
            int.TryParse(x, out i);
            ServoHelper.LastUpdate = DateTime.Now;
            ServoHelper.SetPosition = i;


        }

        public bool IsReusable { get; private set; }
    }

    public class Servo : IHttpHandler {
        public void ProcessRequest(HttpContext context) {
            context.Response.Write(string.Format("<time>{0}</time>", ServoHelper.LastUpdate));
            context.Response.Write(string.Format("<val>{0}</val>", ServoHelper.SetPosition));
        }

        public bool IsReusable { get; private set; }
    }
}