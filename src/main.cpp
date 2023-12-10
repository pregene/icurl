#include "project.h"
#include "curl_pool.h"
#include <unistd.h>
//#include "socket.h"

/*
http://ticket.yes24.com/New/Main.aspx
HTTP_REFERER		.yes24.com	/	Session	12					Medium
RecentViewGoods		.yes24.com	/	2023-12-10T18:00:28.000Z	15					Medium
RecentViewInfo	NotCookie%3DY%26Interval%3D5	.yes24.com	/	2023-12-10T18:00:28.000Z	42					Medium
PCID	17021448285905538836455	.yes24.com	/	2025-01-12T18:00:28.590Z	27					Medium
_fwb	97gg52bHYfpC0K9t0vdr80.1702144828595	.yes24.com	/	2025-01-12T18:00:28.595Z	40					Medium
_gcl_au	1.1.770636897.1702144829	.yes24.com	/	2024-03-08T18:00:28.000Z	31					Medium
_ga_FJT6RQ6VPQ	GS1.1.1702144828.1.0.1702144828.60.0.0	.yes24.com	/	2025-01-12T18:00:28.719Z	52					Medium
_abx__ioBYurVi4UKyZbh859fByw	JTdCJTIyZmlyc3RQYXJ0eUlkJTIyJTNBJTIyYWY4MzBhMjQtMmUyMS00ZTViLTgzZTktZTFiNDViZTFmNzM0JTIyJTJDJTIyaXNTZW5kRmlyc3RQYXJ0eSUyMiUzQXRydWUlMkMlMjJsYXN0Rmlyc3RPcGVuSWQlMjIlM0ElMjIxNzAyMTQ0ODI4NjIyJTNBYmMxOTBhM2QtNjM4OC00MWYzLWIxYTEtZGE2MWMwY2I2MTI4JTIyJTJDJTIybGFzdEV2ZW50TG9nSWQlMjIlM0ElMjIxNzAyMTQ0ODI4NzQ5JTNBYzI2YTM3OWUtYTBlYi00YTQ0LWFjMzQtNmRhZjY5YjQ4ZDBhJTIyJTJDJTIybGFzdERhaWx5Rmlyc3RPcGVuVGltZSUyMiUzQTE3MDIxNDQ4Mjg2MjIlMkMlMjJzZXNzaW9uJTIyJTNBJTdCJTIyc2Vzc2lvbklkJTIyJTNBJTIyYTc2NDEyYWUtZDIzOS00OGY5LTg4NDAtNDAyOThmYzBkZGFkJTIyJTJDJTIybGFzdFVwZGF0ZSUyMiUzQTE3MDIxNDQ4Mjg2MjIlN0QlMkMlMjJ1c2VySWQlMjIlM0FudWxsJTJDJTIyZXZlbnRPcmRlck5vJTIyJTNBNiUyQyUyMnVzZXJQcm9wZXJ0eSUyMiUzQSU3QiUyMnVzZXJQcm9wZXJ0eSUyMiUzQSU1QiU1RCUyQyUyMnNuYXBzaG90JTIyJTNBbnVsbCU3RCU3RA==	.yes24.com	/	2024-12-08T18:00:28.000Z	784			Lax		Medium
_ga	GA1.2.936994434.1702144829	.yes24.com	/	2025-01-12T18:01:15.258Z	29					Medium
_gid	GA1.2.401046318.1702144829	.yes24.com	/	2023-12-10T18:01:15.000Z	30					Medium
_gat_UA-30322978-1	1	.yes24.com	/	2023-12-09T18:01:28.000Z	19					Medium
ASP.NET_SessionId	yvllroavy1xkluwja1d1czln	ticket.yes24.com	/	Session	41	✓		Lax		Medium
__utma	186092716.936994434.1702144829.1702144875.1702144875.1	.ticket.yes24.com	/	2025-01-12T18:01:15.167Z	60					Medium
__utmc	186092716	.ticket.yes24.com	/	Session	15					Medium
__utmz	186092716.1702144875.1.1.utmcsr=yes24.com|utmccn=(referral)|utmcmd=referral|utmcct=/Main/default.aspx	.ticket.yes24.com	/	2024-06-09T06:01:15.000Z	107					Medium
__utmt	1	.ticket.yes24.com	/	2023-12-09T18:11:15.000Z	7					Medium
__utmb	186092716.1.10.1702144875	.ticket.yes24.com	/	2023-12-09T18:31:15.000Z	31					Medium
_gat_UA-166644337-1	1	.yes24.com	/	2023-12-09T18:02:15.000Z	20					Medium
_fbp	fb.1.1702144875293.1430324753	.yes24.com	/	2024-03-08T18:01:15.000Z	33			Lax		Medium
_ga_719LSSZFC3	GS1.2.1702144875.1.0.1702144875.60.0.0	.yes24.com	/	2025-01-12T18:01:15.495Z	52					Medium
*/

/*
YesTicket		ticket.yes24.com	/	2023-12-09T17:01:24.000Z	58				http://yes24.com	Medium
ServiceCookies		yes24.com	/	2023-12-09T17:01:24.000Z	81				http://yes24.com	Medium
ticketuser		yes24.com	/	2023-12-09T18:00:24.000Z	76				http://yes24.com	Medium
*/
int main(int argc, char* argv[])
{
  CURLSession session;
  //session.OpenURL("https://www.yes24.com/Main/default.aspx");
  //session.SetVerbose(1);
  session.SetRedirect(1);
  session.SetWriteFile("temp/1.res");
  session.SetHeaderFile("data/linux_header");
  session.SetCookieFile("data/cookies.txt");
  //session.OpenURL("https://www.yes24.com");
  session.OpenURL("http://ticket.yes24.com/New/Main.aspx");

  //printf("[%s:%d]\n", session.GetURL().c_str(), session.GetReturnCode());

  //printf("%s\n", session.GetHTTPHeader("Content-Type").c_str());
  //printf("[HEADER]\n%s", session.GetHeaderString().c_str());

  //printf("%s\n", session.GetJARCookie("HTTP_REFERER").c_str());
  //printf("%s\n", session.GetJARCookies().c_str());
  //printf("%s\n", session.GetBody().c_str());

  session.SetWriteFile("temp/2.res");
  session.SetHeaderFile("data/linux_header");
  session.SetCookieFile("data/cookies.txt");
  session.PostURLEncode("http://localhost:8080/", "a=Hello#!&b=1", 13);
  //session.PostURL("http://localhost:8080/", "post.data");

  printf("[%s:%d]\n", session.GetURL().c_str(), session.GetReturnCode());

  //printf("%s\n", session.GetHTTPHeader("Content-Type").c_str());
  printf("[HEADER]\n%s", session.GetHeaderString().c_str());

  pause();
  return 0;
}
