#include "signals.h"
#include <sys/ioctl.h>
#include <signal.h>

void HandleSigwinch(STREAM *Pipe)
{
    struct winsize w;
		char *Tempstr=NULL;

    ioctl(0, TIOCGWINSZ, &w);

		ScreenRows=w.ws_row;
		ScreenCols=w.ws_col;
		if (GlobalFlags & HAS_STATUSBAR) SetupStatusBars();
		else
		{
//		w.ws_row--;	

    if (Pipe) ioctl(Pipe->out_fd, TIOCSWINSZ, &w);
		}
		DestroyString(Tempstr);
}


void HandleSignal(int sig)
{
int wid, len;

if (sig==SIGWINCH) GlobalFlags |= GOT_SIGWINCH;
if (sig==SIGTERM) GlobalFlags |= GOT_SIGTERM;
if (sig==SIGINT) GlobalFlags |= GOT_SIGINT;
}



void PropogateSignals(STREAM *Pipe)
{
int PeerPID;

if (! Pipe) return;

PeerPID=atoi(STREAMGetValue(Pipe,"PeerPID"));
if (GlobalFlags & GOT_SIGWINCH) 
{
	HandleSigwinch(Pipe);
	//kill(PeerPID,SIGWINCH);
}

if (GlobalFlags & GOT_SIGTERM) kill(PeerPID,SIGTERM);
if (GlobalFlags & GOT_SIGINT) kill(PeerPID,SIGINT);
GlobalFlags &= ~(GOT_SIGWINCH | GOT_SIGTERM | GOT_SIGINT);
}

