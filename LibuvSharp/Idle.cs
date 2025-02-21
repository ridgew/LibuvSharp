using System;
using System.Runtime.InteropServices;

namespace LibuvSharp
{
	public class Idle : StartableCallbackHandle
	{
		[DllImport(NativeMethods.libuv, CallingConvention = CallingConvention.Cdecl)]
		static extern int uv_idle_init(IntPtr loop, IntPtr idle);

		[DllImport(NativeMethods.libuv, CallingConvention = CallingConvention.Cdecl)]
		static extern int uv_idle_start(IntPtr idle, uv_handle_cb callback);

		[DllImport(NativeMethods.libuv, CallingConvention = CallingConvention.Cdecl)]
		static extern int uv_idle_stop(IntPtr idle);

		public Idle()
			: this(Loop.Constructor)
		{
		}

		public Idle(Loop loop)
			: base(loop, HandleType.UV_IDLE, uv_idle_init)
		{
		}

		public override void Start()
		{
			Invoke(uv_idle_start);
		}

		public override void Stop()
		{
			Invoke(uv_idle_stop);
		}
	}
}

