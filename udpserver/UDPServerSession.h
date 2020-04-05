#ifndef UDP_SESSION_H_
#define UDP_SESSION_H_
#include "IStreamSession.h"
#include <string.h>
#include <atomic>
#include <mutex>
#include <list>

class CUDPServer;
class UdpServerSession;
struct EventItem;
/*���̰߳󶨣����߳�Ϊ��Ӧ��ϵ*/
class EventContext
{
public:
    int pipe_write_;
    int pipe_read_;
    int event_size_;                   /*�������߳��и��ص���������EventContext�������̰߳�*/
    std::mutex list_lock_;
    std::list<UdpServerSession*> event_list_;
    struct event *listener_event_;
    struct event_base* event_base_;
};

struct EventInfo
{
	EventInfo(){
		memset(this, 0x00, sizeof(struct EventInfo));
	}

    CUDPServer *udpserver_;             /*udpserver����֮��ö����ֵ����*/
    void *us_;                          /*ÿ�����󶼻ᴴ��udpsession*/
    struct event *listener_event_;      /*ÿ�����󶼻ᴴ�������¼����������¼�*/
};

/*�߳��ϵ�����*/
struct EventItem
{
    EventItem(){
        memset(this, 0x00, sizeof(struct EventItem));
    }
    int  event_state_;                  /*�¼�����*/
    EventContext *event_content_;       /*��¼�����������߳�*/
    EventInfo *event_info_;             /*ÿ���������event_new�ӿڴ������¼���Ϣ*/
};

class UdpServerSession : public IStreamSession
{
public:
    UdpServerSession();
    ~UdpServerSession();
public:
    virtual void AddRef();
    virtual void Release();
    //set callback function
    virtual void SetSessionAction(IStreamAction *action);
    virtual void  SetRequestSession(int session);
    virtual int   GetRequestSession();
    virtual void  SetResponseSession(void *session);
    virtual void* GetResponseSession();
    virtual int SetRequestTimeout(long second);
    virtual const char * GetSrcAddress(){};
    virtual int GetSrcPort(){};
    virtual const char * GetDstAddress(){};
    virtual int GetDstPort(){};

	long GetRequestTimeOut();

    //get event item
    EventItem* GetEventItem(){ return &event_item_; }
    //get event info
    EventInfo* GetEventInfo(){ return &event_info_; }
    //create udpSession
    static UdpServerSession* CreateUdpSession();
    //delete udpSession
    void DeleteUdpSession(UdpServerSession *us);
    IStreamAction *callback_fun_;
    int fd_;
public:
	long timeout_seconds_;
    void *application_us_;
    std::atomic<int> ref_count_;
    struct EventItem event_item_;
    struct EventInfo event_info_;
};

#endif

