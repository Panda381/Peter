
#define JERROR_H
#define JMSG_LASTMSGCODE 0

#define ERREXIT(cinfo,code) { m_Error = TRUE; }
#define ERREXIT1(cinfo,code,p1) { m_Error = TRUE; }
#define ERREXIT2(cinfo,code,p1,p2) { m_Error = TRUE; }
#define ERREXIT3(cinfo,code,p1,p2,p3) { m_Error = TRUE; }
#define ERREXIT4(cinfo,code,p1,p2,p3,p4) { m_Error = TRUE; }
#define ERREXITS(cinfo,code,str) { m_Error = TRUE; }
#define MAKESTMT(stuff)		do { stuff } while (0)
#define WARNMS(cinfo,code) {}
#define WARNMS1(cinfo,code,p1) {}
#define WARNMS2(cinfo,code,p1,p2) {}
#define TRACEMS(cinfo,lvl,code) {}
#define TRACEMS1(cinfo,lvl,code,p1) {}
#define TRACEMS2(cinfo,lvl,code,p1,p2) {}
#define TRACEMS3(cinfo,lvl,code,p1,p2,p3) MAKESTMT({})
#define TRACEMS4(cinfo,lvl,code,p1,p2,p3,p4) MAKESTMT({})
#define TRACEMS5(cinfo,lvl,code,p1,p2,p3,p4,p5) MAKESTMT({})
#define TRACEMS8(cinfo,lvl,code,p1,p2,p3,p4,p5,p6,p7,p8) MAKESTMT({})
#define TRACEMSS(cinfo,lvl,code,str) {}
