/*************************************************************************
	> File Name: server_re_drew.c
	> Author: lbh
	> Mail: 2052658718@qq.com
	> Created Time: 2020年06月14日 星期日 20时29分32秒
 ************************************************************************/

#include "head.h"
#include "game.h"
#define MAX 50

extern struct User *rteam,*bteam;
extern WINDOW *Football, *Football_t;
extern struct Bpoint ball;
extern struct BallStatus ball_status;
extern struct Map court;

void re_drew_player(int team, char *name, struct Point *loc) {
	char p = 'K';
	char blank[20] = {0};
	wattron(Football, COLOR_PAIR((team ? 6 : 2)));
	w_gotoxy_putc(Football, loc->x, loc->y, p);
	w_gotoxy_puts(Football, loc->x + 1, loc->y - 1, name);
	wattroff(Football, COLOR_PAIR((team ? 6 : 2)));
}

void re_drew_team(struct User *team) {
	for (int i = 0; i < MAX ; i++) {
		if (!team[i].online) continue;
		re_drew_player(team[i].team, team[i].name, &team[i].loc);
	}
}

void re_drew_ball() {
	double t = 0.1;
	if (ball_status.v.x != 0 || ball_status.v.y != 0) {
		ball.x += ball_status.v.x * 0.1 + ball_status.a.x * 0.5 * 0.01;
		ball.y += ball_status.v.y * 0.1 + ball_status.a.y * 0.5 * 0.01;
		ball_status.v.x += ball_status.a.x * 0.1;
		ball_status.v.y += ball_status.a.y * 0.1;
	}
	if (abs(ball_status.v.y) < 1 && abs(ball_status.v.x) < 1) {
		ball_status.v.x = ball_status.v.y = 0;
		ball_status.a.x = ball_status.a.y = 0;
	}
	if (ball_status.v.x || ball_status.v.y) {
		char tmp[512] = {0};
		sprintf(tmp, "a(%lf, %lf) v(%lf, %lf)", ball_status.a.x, ball_status.a.y, ball_status.v.x, ball_status.v.y);
		Show_Message( , NULL, tmp, 1);
	}
	if (ball.x <= 0 || ball.x >= 114 || ball.y <= 0 || ball.y >= 24) {
		ball.x = court.width / 2;
		ball.y = court.height / 2;
		ball_status.v.x = ball_status.v.y = 0;
		ball_status.a.x = ball_status.a.y = 0;
	}
	w_gotoxy_putc(Football, (int)ball.x, (int)ball.y, 'o');
}

void re_drew() {
	werase(Football_t);
	box(Football_t, 0, 0);
	box(Football, 0, 0);
	re_drew_team(rteam);
	re_drew_team(bteam);
	re_drew_ball();
	wrefresh(Football);
	wrefresh(Football_t);
}
