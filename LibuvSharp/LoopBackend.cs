using System;
using System.Runtime.InteropServices;

namespace LibuvSharp
{
	public class LoopBackend
	{
		IntPtr nativeHandle;

		internal LoopBackend(Loop loop)
		{
			nativeHandle = loop.NativeHandle;
		}

		[DllImport(NativeMethods.libuv, CallingConvention = CallingConvention.Cdecl)]
		static extern int uv_backend_fd(IntPtr loop);

		public int FileDescriptor {
			get {
				return uv_backend_fd(nativeHandle);
			}
		}

		[DllImport(NativeMethods.libuv, CallingConvention = CallingConvention.Cdecl)]
		static extern int uv_backend_timeout(IntPtr loop);

		public int Timeout {
			get {
				return uv_backend_timeout(nativeHandle);
			}
		}
	}
}

