/*
    This server is modified from 5F free residential open source, Prace open source
    To the makers of Prace:[Ghost]Rui ushio_p [Ghost]Dylan
    Tribute to the makers of the free settlement :[ITC]dyq [ITC]fangye [ITC]Super_wlc [ITC
    Special thanks to ryddawn and technical advisor [Fire]KiVen OBJ:JoshenKM;

								The MIT License  
								
	Copyright (c) <2019-2020> YuCarl77 & ryddawn 
	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in
	all copies or substantial portions of the Software.  
	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
	THE SOFTWARE.  
	==================================
	��Ȩ(c) <2019-2020> YuCarl77 & ryddawn 

	ʹ�ø�����֤����������������Ȩ�ޣ���ѣ��κ��˿��Եõ����������������ĵ���һ��������
	���Ҿ�Ӫ�����������κ����ƣ����������Ƶ�ʹ�á����ơ��޸ġ��ϲ������桢���С����Ŵ���֤�顢���߳��۸������Ŀ�����Ȩ����
	ͬʱ���������Щ�������û�������ЩȨ����ʹ����������������  
	
	���ϵİ�Ȩ֪ͨ��Ȩ��֪ͨӦ�ð��������и������Ŀ����л�������������������ʵ�����С�
	
	�������������������ṩ��û���κ���ʽ�ĵ�������������ȷ�ػ��߰����ģ�������Щ���ǲ�����ҵ���ʵĵ��������ơ�
	�ʺ�һ���ض�����;���Ҳ����ַ������ߺͰ�Ȩ���������κγ��϶�ʹ�ø������漰���κ�Ҫ���𺦻����������ζ���Ӧ����
	�����������������û���ֻ�Ǻ�ͬ��ʽ��������Ȩ����������ʽ�������������������÷�Χ�ڡ������������ϵ����������ʹ�û�����������������������Ϊ��  
	=====================================   
*/



#define MAX_CAMERAS 30 //��ͷ�������
// #define CAMERA_DIALOG_MAIN 520 //������ID
#define CAMERA_DIALOG_ROWS 10 //ÿҳ��ʾ������


#define MOVE_FORWARD    		(1)
#define MOVE_BACK       		(2)
#define MOVE_LEFT       		(3)
#define MOVE_RIGHT      		(4)
#define MOVE_FORWARD_LEFT       (5)
#define MOVE_FORWARD_RIGHT      (6)
#define MOVE_BACK_LEFT          (7)
#define MOVE_BACK_RIGHT         (8)


forward PlayCamera(playerid, SID, EID);

enum e_PLAYER_FLY_MODE {
    flyMode,
    flyType,
    flyObject,
    flyDirection,
    flyKeys[2],
    flyTick,
    Float:flyMovementSpeed
};
new g_FlyMode[MAX_PLAYERS][e_PLAYER_FLY_MODE];


enum Cinfo {
    CameraStatus[MAX_CAMERAS],
    Float:CameraX[MAX_CAMERAS],
    Float:CameraY[MAX_CAMERAS],
    Float:CameraZ[MAX_CAMERAS],
    Float:CameraAtX[MAX_CAMERAS],
    Float:CameraAtY[MAX_CAMERAS],
    Float:CameraAtZ[MAX_CAMERAS],
    MoveTime[MAX_CAMERAS]
}
new CameraInfo[MAX_PLAYERS][Cinfo]; //��¼��ͷ��Ϣ

enum CreateInfo {
    CreateStatus, //����״̬.0(��),1(��һ������),2(�ڶ�������)
    Float:CreateX,
    Float:CreateY,
    Float:CreateZ,
    Float:CreateAtX,
    Float:CreateAtY,
    Float:CreateAtZ
}
new CreateCamera[MAX_PLAYERS][CreateInfo]; //��ʱ��¼������ͷ��Ϣ

enum PCI //PlayerCameraInfo
{
    Using, //ʹ���е�ID
    Page, //��¼ҳ��
    StartID, //��һ�����ŵľ�ͷID
    EndID, //�������ŵľ�ͷID
    cTimerID, //��ʱ��ID
    TF //��¼����
}
new PlayerCamera[MAX_PLAYERS][PCI];

stock Initialize_Camera() //��ʼ����ͷϵͳ
{
    printf("�ӽ�ϵͳ - By YuCarl77 & ryddawn");
    for (new i = 0; i < MAX_PLAYERS; i++) {
        ResetCreateInfo(i);
        ResetCameraInfo(i);
        PlayerCamera[i][Using] = -1;
        PlayerCamera[i][Page] = 1;
        PlayerCamera[i][StartID] = -1;
        PlayerCamera[i][EndID] = -1;
    }
}

stock Camera_OnPlayerConnect(const playerid) { //�������ʱ�����һ����Ҿ�ͷ��Ϣ
    ResetCreateInfo(playerid);
    g_FlyMode[playerid][flyMode] = 1;
    g_FlyMode[playerid][flyType] = 0;
    g_FlyMode[playerid][flyObject] = INVALID_OBJECT_ID;
    g_FlyMode[playerid][flyDirection] = 0;
    g_FlyMode[playerid][flyKeys][0] = 0;
    g_FlyMode[playerid][flyKeys][1] = 0;
    g_FlyMode[playerid][flyTick] = 0;
    g_FlyMode[playerid][flyMovementSpeed] = 50.0;

    PlayerCamera[playerid][Using] = -1;
    PlayerCamera[playerid][Page] = 1;
    PlayerCamera[playerid][StartID] = -1;
    PlayerCamera[playerid][EndID] = -1;
}
stock Camera_OnPlayerDisConnect(const playerid) { //�������ʱ���OBJ
    if(g_FlyMode[playerid][flyType] != 0)
    {
        DestroyDynamicObject(g_FlyMode[playerid][flyObject]);
        g_FlyMode[playerid][flyObject] = INVALID_OBJECT_ID;
    }
}


CMD:camera(playerid, params[]) { //�������cameraָ��
    if(PlayerInfo[playerid][tvzt] || IsPlayerInAnyVehicle(playerid))
    {
        SendClientMessage(playerid, Color_White, "[�����]�޷���TV״̬������״̬�µ���");
        return 1;
    }
    new msg[128];
    format(msg, sizeof msg, "====== ���ž�ͷ ======\n====== ֹͣ���� ======\n====== �༭��ͷ ======\n====== �ӽ����� ======\n====== �ӽ����� ======");
    Dialog_Show(playerid, CAMERA_DIALOG_MAIN, DIALOG_STYLE_LIST, "�ҵľ�ͷ", msg, "ȷ��", "ȡ��");
    return 1;
}
CMD:ssj(playerid, params[]) return cmd_camera(playerid, params); //��ָ��ָ��
CMD:jt(playerid, params[]) return cmd_camera(playerid, params);
CMD:cam(playerid, params[]) return cmd_camera(playerid, params);

// public OnDialogResponse(playerid, dialogid, response, listitem, inputtext[]) {

//     if(dialogid == CAMERA_DIALOG_MAIN) //������
//     {
//         if(response) {
//             switch (listitem) {
//                 case 0:{
//                     Camera_SelectDialog(playerid, PlayerCamera[playerid][Page]);
//                     return 1;
//                 }
//                 case 1:{
//                     KillTimer(PlayerCamera[playerid][cTimerID]);
//                     SetCameraBehindPlayer(playerid);
//                     return 1;
//                 }
//                 case 2:{
//                     Camera_ShowPlayerDialog(playerid, PlayerCamera[playerid][Page]);
//                     return 1;
//                 }
//             }
//         }
//     }
//     if(dialogid == CAMERA_DIALOG_MAIN + 1) //��������
//     {
//         if(response) {
//             for (new i; i < CAMERA_DIALOG_ROWS; i++) {
//                 if(listitem == i) {
//                     new row;
//                     new title[32];
// 					new msg[1024];
//                     row = (PlayerCamera[playerid][Page] - 1) * CAMERA_DIALOG_ROWS + i;
//                     if(CameraInfo[playerid][CameraStatus][row] == 0) format(msg, sizeof(msg), "������ͷ", msg);
//                     else if(CameraInfo[playerid][CameraStatus][row] == 1) format(msg, sizeof(msg), "ɾ����ͷ\n�鿴\nMoveTime[%d]", CameraInfo[playerid][MoveTime][row]);
//                     format(title, sizeof(title), "��ͷ[%d]", row);
//                     PlayerCamera[playerid][Using] = row;
//                     ShowPlayerDialog(playerid, CAMERA_DIALOG_MAIN + 2, DIALOG_STYLE_LIST, title, msg, "ȷ��", "ȡ��");
//                     return 1;
//                 }
//             }
//             switch (listitem) {
//                 case 10:{
//                     PlayerCamera[playerid][Page]--;
//                     Camera_ShowPlayerDialog(playerid, PlayerCamera[playerid][Page]);
//                     return 1;
//                 }
//                 case 11:{
//                     PlayerCamera[playerid][Page]++;
//                     Camera_ShowPlayerDialog(playerid, PlayerCamera[playerid][Page]);
//                     return 1;
//                 }
//             }
//         }
//     }
//     if(dialogid == CAMERA_DIALOG_MAIN + 2) //��ʾ��ͷ��Ϣ
//     {
//         if(response) {
//             new nUsing = PlayerCamera[playerid][Using];
//             switch (listitem) {
//                 case 0:{
//                     if(CameraInfo[playerid][CameraStatus][nUsing] == 0) {
//                         CreateCamera[playerid][CreateStatus] = 1;
//                         SCM(playerid, Color_White, "[�����]��ʼ������ͷ");
//                         return 1;
//                     }
//                     if(CameraInfo[playerid][CameraStatus][nUsing] == 1) {
//                         DeletCamera(playerid, nUsing);
//                         Camera_ShowPlayerDialog(playerid, PlayerCamera[playerid][Page]);
//                         return 1;
//                     }
//                 }
//                 case 1:{
//                     SetPlayerCameraPos(playerid, CameraInfo[playerid][CameraX][nUsing], CameraInfo[playerid][CameraY][nUsing], CameraInfo[playerid][CameraZ][nUsing]);
//                     SetPlayerCameraLookAt(playerid, CameraInfo[playerid][CameraAtX][nUsing], CameraInfo[playerid][CameraAtY][nUsing], CameraInfo[playerid][CameraAtZ][nUsing]);
//                     CreateCamera[playerid][CreateStatus] = 3;
//                     SCM(playerid, Color_White, "[�����]��Y����");
//                     return 1;
//                 }
//                 case 2:{
//                     ShowPlayerDialog(playerid, CAMERA_DIALOG_MAIN + 3, DIALOG_STYLE_INPUT, "�޸�movetime", "�����޸���ֵ", "ȷ��", "ȡ��");
//                     return 1;
//                 }

//             }
//         }
//     }
//     if(dialogid == CAMERA_DIALOG_MAIN + 3) //�޸�movetime����
//     {
//         if(response) {
//             new id = PlayerCamera[playerid][Using];
//             CameraInfo[playerid][MoveTime][id] = strval(inputtext);
//             Camera_ShowPlayerDialog(playerid, PlayerCamera[playerid][Page]);
//             return 1;
//         }
//     }
//     if(dialogid == CAMERA_DIALOG_MAIN + 4) //ѡ�񲥷ž�ͷ����
//     {
//         if(response) {
//             for (new i; i < CAMERA_DIALOG_ROWS; i++) {
//                 if(listitem == i) {
//                     new row;
//                     row = (PlayerCamera[playerid][Page] - 1) * CAMERA_DIALOG_ROWS + i;
//                     if(CreateCamera[playerid][CreateStatus] == 4) {
//                         PlayerCamera[playerid][EndID] = row;
//                         PlayerCamera[playerid][cTimerID] = SetTimerEx("PlayCamera", 3000, false, "iii", playerid, PlayerCamera[playerid][StartID], PlayerCamera[playerid][EndID]);

//                         if(PlayerCamera[playerid][StartID] < PlayerCamera[playerid][EndID]) {
//                             PlayerCamera[playerid][TF] = 1;
//                         } else {
//                             PlayerCamera[playerid][TF] = 2;
//                         }
//                         ResetCreateInfo(playerid);
//                         return 1;
//                     } else {
//                         PlayerCamera[playerid][StartID] = row;
//                         CreateCamera[playerid][CreateStatus] = 4;
//                         Camera_SelectDialog(playerid, PlayerCamera[playerid][Page]);
//                     }
//                     return 1;
//                 }
//             }
//             switch (listitem) {
//                 case 10:{
//                     PlayerCamera[playerid][Page]--;
//                     Camera_SelectDialog(playerid, PlayerCamera[playerid][Page]);
//                     return 1;
//                 }
//                 case 11:{
//                     PlayerCamera[playerid][Page]++;
//                     Camera_SelectDialog(playerid, PlayerCamera[playerid][Page]);
//                     return 1;
//                 }
//             }
//         }
//     }

//     return 1;
// }

// public OnPlayerKeyStateChange(playerid, newkeys, oldkeys) {
//     if(newkeys & KEY_YES) { //����Y 
//         CreatingCamera(playerid);
//         return 1;
//     }
//     return 1;
// }

public PlayCamera(playerid, SID, EID) {
    if(PlayerCamera[playerid][TF] == 1) {
        InterpolateCameraPos(playerid, CameraInfo[playerid][CameraX][SID], CameraInfo[playerid][CameraY][SID], CameraInfo[playerid][CameraZ][SID], \
            CameraInfo[playerid][CameraX][SID + 1], CameraInfo[playerid][CameraY][SID + 1], CameraInfo[playerid][CameraZ][SID + 1], CameraInfo[playerid][MoveTime][SID], CAMERA_MOVE);
        InterpolateCameraLookAt(playerid, CameraInfo[playerid][CameraAtX][SID], CameraInfo[playerid][CameraAtY][SID], CameraInfo[playerid][CameraAtZ][SID], \
            CameraInfo[playerid][CameraAtX][SID + 1], CameraInfo[playerid][CameraAtY][SID + 1], CameraInfo[playerid][CameraAtZ][SID + 1], CameraInfo[playerid][MoveTime][SID], CAMERA_MOVE);
        if(PlayerCamera[playerid][StartID] == PlayerCamera[playerid][EndID]) {
            SetCameraBehindPlayer(playerid);
            return 1;
        } else {
            PlayerCamera[playerid][StartID]++;
            new stid = PlayerCamera[playerid][StartID];
            if(CameraInfo[playerid][CameraStatus][stid] == 0) {
                SetCameraBehindPlayer(playerid);
                return 1;
            } else {
                PlayerCamera[playerid][cTimerID] = SetTimerEx("PlayCamera", CameraInfo[playerid][MoveTime][SID] - 500, false, "iii", playerid, PlayerCamera[playerid][StartID], PlayerCamera[playerid][EndID]);
                return 1;
            }
        }
    } else {
        InterpolateCameraPos(playerid, CameraInfo[playerid][CameraX][SID], CameraInfo[playerid][CameraY][SID], CameraInfo[playerid][CameraZ][SID], \
            CameraInfo[playerid][CameraX][SID - 1], CameraInfo[playerid][CameraY][SID - 1], CameraInfo[playerid][CameraZ][SID - 1], CameraInfo[playerid][MoveTime][SID - 1], CAMERA_MOVE);
        InterpolateCameraLookAt(playerid, CameraInfo[playerid][CameraAtX][SID], CameraInfo[playerid][CameraAtY][SID], CameraInfo[playerid][CameraAtZ][SID], \
            CameraInfo[playerid][CameraAtX][SID - 1], CameraInfo[playerid][CameraAtY][SID - 1], CameraInfo[playerid][CameraAtZ][SID - 1], CameraInfo[playerid][MoveTime][SID - 1], CAMERA_MOVE);
        PlayerCamera[playerid][StartID]--;
        // printf("%d %d", PlayerCamera[playerid][StartID], PlayerCamera[playerid][EndID]);
        if(PlayerCamera[playerid][StartID] + 1 == PlayerCamera[playerid][EndID]) {
            SetCameraBehindPlayer(playerid);
            return 1;
        } else {

            PlayerCamera[playerid][cTimerID] = SetTimerEx("PlayCamera", CameraInfo[playerid][MoveTime][SID] - 500, false, "iii", playerid, PlayerCamera[playerid][StartID], PlayerCamera[playerid][EndID]);
            return 1;
        }
    }
}

stock ResetCreateInfo(const playerid) //������Ϣ��ʼ��
{
    CreateCamera[playerid][CreateStatus] = 0;
    CreateCamera[playerid][CreateX] = 0;
    CreateCamera[playerid][CreateY] = 0;
    CreateCamera[playerid][CreateZ] = 0;
    CreateCamera[playerid][CreateAtX] = 0;
    CreateCamera[playerid][CreateAtY] = 0;
    CreateCamera[playerid][CreateAtZ] = 0;
    return 1;
}
stock ResetCameraInfo(const playerid) //camera��Ϣ��ʼ��
{
    for (new i = 0; i < MAX_CAMERAS; i++) {
        CameraInfo[playerid][CameraStatus][i] = 0;
        CameraInfo[playerid][CameraX][i] = 0;
        CameraInfo[playerid][CameraY][i] = 0;
        CameraInfo[playerid][CameraZ][i] = 0;
        CameraInfo[playerid][CameraAtX][i] = 0;
        CameraInfo[playerid][CameraAtY][i] = 0;
        CameraInfo[playerid][CameraAtZ][i] = 0;
        CameraInfo[playerid][MoveTime][i] = 5000;
    }
    return 1;
}
stock NewCamera(const playerid, const CameraID, Float:X, Float:Y, Float:Z, Float:AtX, Float:AtY, Float:AtZ) //�½���ͷ
{
    CameraInfo[playerid][CameraStatus][CameraID] = 1;
    CameraInfo[playerid][CameraX][CameraID] = X;
    CameraInfo[playerid][CameraY][CameraID] = Y;
    CameraInfo[playerid][CameraZ][CameraID] = Z;
    CameraInfo[playerid][CameraAtX][CameraID] = AtX;
    CameraInfo[playerid][CameraAtY][CameraID] = AtY;
    CameraInfo[playerid][CameraAtZ][CameraID] = AtZ;
    return 1;
}
stock DeletCamera(const playerid, const CameraID) //ɾ����ͷ
{
    CameraInfo[playerid][CameraStatus][CameraID] = 0;
    CameraInfo[playerid][CameraX][CameraID] = 0;
    CameraInfo[playerid][CameraY][CameraID] = 0;
    CameraInfo[playerid][CameraZ][CameraID] = 0;
    CameraInfo[playerid][CameraAtX][CameraID] = 0;
    CameraInfo[playerid][CameraAtZ][CameraID] = 0;
    CameraInfo[playerid][CameraAtY][CameraID] = 0;
    return 1;
}
stock CreatingCamera(const playerid) //������ͷ
{
    if(CreateCamera[playerid][CreateStatus] != 0) {
        if(CreateCamera[playerid][CreateStatus] == 1) {
            GetPlayerCameraPos(playerid, CreateCamera[playerid][CreateX], CreateCamera[playerid][CreateY], CreateCamera[playerid][CreateZ]);
            CreateCamera[playerid][CreateStatus] = 2;
            SCM(playerid, Color_White, "[�����]������ͷλ�óɹ�");
            SCM(playerid, Color_White, "[�����]�ڶ��� - ������ͷ����λ��");
            return 1;
        }
        if(CreateCamera[playerid][CreateStatus] == 2) {
            GetPlayerCameraPos(playerid, CreateCamera[playerid][CreateAtX], CreateCamera[playerid][CreateAtY], CreateCamera[playerid][CreateAtZ]);
            NewCamera(playerid, PlayerCamera[playerid][Using], CreateCamera[playerid][CreateX], CreateCamera[playerid][CreateY], CreateCamera[playerid][CreateZ], CreateCamera[playerid][CreateAtX], CreateCamera[playerid][CreateAtY], CreateCamera[playerid][CreateAtZ]);
            Camera_ShowPlayerDialog(playerid, PlayerCamera[playerid][Page]);
            SCM(playerid, Color_White, "[�����]�þ�ͷ�����ɹ�");
            SetPlayerCustomCamera(playerid,0);
            ResetCreateInfo(playerid);
            return 1;
        }
        // if(CreateCamera[playerid][CreateStatus] == 1) {
        //     GetPlayerPos(playerid, CreateCamera[playerid][CreateX], CreateCamera[playerid][CreateY], CreateCamera[playerid][CreateZ]);
        //     CreateCamera[playerid][CreateStatus] = 2;
        //     SCM(playerid, Color_White, "[�����]������ͷλ�óɹ����밴Y��ǰ��ͷ�鿴λ�ã������¥.����");
        //     return 1;
        // }
        // if(CreateCamera[playerid][CreateStatus] == 2) {
        //     GetPlayerPos(playerid, CreateCamera[playerid][CreateAtX], CreateCamera[playerid][CreateAtY], CreateCamera[playerid][CreateAtZ]);
        //     SCM(playerid, Color_White, "[�����]������ͷλ�óɹ�");
        //     NewCamera(playerid, PlayerCamera[playerid][Using], CreateCamera[playerid][CreateX], CreateCamera[playerid][CreateY], CreateCamera[playerid][CreateZ], CreateCamera[playerid][CreateAtX], CreateCamera[playerid][CreateAtY], CreateCamera[playerid][CreateAtZ]);
        //     ResetCreateInfo(playerid);
        //     Camera_ShowPlayerDialog(playerid, PlayerCamera[playerid][Page]);
        //     return 1;
        // }
        if(CreateCamera[playerid][CreateStatus] == 3) {
            SetCameraBehindPlayer(playerid);
            ResetCreateInfo(playerid);
            return 1;
        }
    }
    return 1;
}
stock Camera_ShowPlayerDialog(const playerid, page) //�������ʾ��ͷ����
{
    if(PlayerInfo[playerid][tvzt] || IsPlayerInAnyVehicle(playerid))
    {
        SendClientMessage(playerid, Color_White, "[�����]�޷���TV״̬������״̬�µ���");
        return 1;
    }
    new msg[256], maxpage;
    maxpage = GetMaxPage(MAX_CAMERAS, CAMERA_DIALOG_ROWS);
    if(page < 1) {
        SCM(playerid, Color_White, "[�����]ҳ��С��1,��ת����ҳ");
        page = 1;
        PlayerCamera[playerid][Page] = page;
    }
    if(page > maxpage) {
        SCM(playerid, Color_White, "[�����]ҳ���������ҳ��,��ת��ĩҳ");
        page = maxpage;
        PlayerCamera[playerid][Page] = page;
    }
    new amax, amin;
    amax = (page * CAMERA_DIALOG_ROWS) - 1;
    amin = (page - 1) * CAMERA_DIALOG_ROWS;
    for (new i = amin; i <= amax; i++) {
        if(CameraInfo[playerid][CameraStatus][i] == 1) format(msg, sizeof(msg), "%s��ͷ%d [�Ѵ���]\n", msg, i);
        else format(msg, sizeof(msg), "%s��ͷ%d\n", msg, i);
    }
    format(msg, sizeof(msg), "%s��һҳ\n��һҳ", msg);
    Dialog_Show(playerid, CAMERA_ADJUST, DIALOG_STYLE_LIST, "��ͷ", msg, "ȷ��", "ȡ��");
    return 1;
}
stock Camera_SelectDialog(const playerid, page) //���ѡ�񲥷ŵľ�ͷ����
{
    new msg[256],
        title[64],
        maxpage;
    format(title, sizeof(title), "ѡ���һ�����ŵľ�ͷ");
    if(CreateCamera[playerid][CreateStatus] == 4) format(title, sizeof(title), "ѡ�����һ�����ŵľ�ͷ");
    maxpage = GetMaxPage(MAX_CAMERAS, CAMERA_DIALOG_ROWS);
    if(page < 1) {
        SCM(playerid, Color_White, "[�����]ҳ��С��1,��ת����ҳ");
        page = 1;
        PlayerCamera[playerid][Page] = page;
    }
    if(page > maxpage) {
        SCM(playerid, Color_White, "[�����]ҳ���������ҳ��,��ת��ĩҳ");
        page = maxpage;
        PlayerCamera[playerid][Page] = page;
    }
    new amax, amin;
    amax = (page * CAMERA_DIALOG_ROWS) - 1;
    amin = (page - 1) * CAMERA_DIALOG_ROWS;
    for (new i = amin; i <= amax; i++) {
        if(CameraInfo[playerid][CameraStatus][i] == 1) format(msg, sizeof(msg), "%s��ͷ%d [�Ѵ���]\n", msg, i);
        else format(msg, sizeof(msg), "%s��ͷ%d\n", msg, i);
    }
    format(msg, sizeof(msg), "%s��һҳ\n��һҳ", msg);
    Dialog_Show(playerid, CAMERA_Play, DIALOG_STYLE_LIST, title, msg, "ȷ��", "ȡ��");
    return 1;
}
// stock GetMaxPage(const count, const rows) //����������ÿҳ����,����ҳ��
// {
//     new maxpage;
//     maxpage = count % rows;
//     if(maxpage == 0) {
//         maxpage = count / rows;
//     } else {
//         maxpage = (count / rows) + 1;
//     }
//     return maxpage;
// }
/*stock GetPlayerCameraCount(playerid)//��ȡ��Ҿ�ͷ����
{
	new count = 0;
	for(new i = 0;i < MAX_CAMERAS;i ++)
	{
		if(CameraInfo[playerid][CameraStatus][i] != 0)
		{
			count ++;
		}
	}
	return count;
}*/



stock MovePlayerCamera(const playerid, const Float:CP[3], const Float:FV[3]) {
    new
    Float:NP[3];

    #define OFFSET_X (FV[0]*6000.0)
    #define OFFSET_Y (FV[1]*6000.0)
    #define OFFSET_Z (FV[2]*6000.0)
    switch (g_FlyMode[playerid][flyDirection]) {
        case MOVE_FORWARD:{
            NP[0] = CP[0] + OFFSET_X;
            NP[1] = CP[1] + OFFSET_Y;
            NP[2] = CP[2] + OFFSET_Z;
        }
        case MOVE_BACK:{
            NP[0] = CP[0] - OFFSET_X;
            NP[1] = CP[1] - OFFSET_Y;
            NP[2] = CP[2] - OFFSET_Z;
        }
        case MOVE_LEFT:{
            NP[0] = CP[0] - OFFSET_Y;
            NP[1] = CP[1] + OFFSET_X;
            NP[2] = CP[2];
        }
        case MOVE_RIGHT:{
            NP[0] = CP[0] + OFFSET_Y;
            NP[1] = CP[1] - OFFSET_X;
            NP[2] = CP[2];
        }
        case MOVE_BACK_LEFT:{
            NP[0] = CP[0] + (-OFFSET_X - OFFSET_Y);
            NP[1] = CP[1] + (-OFFSET_Y + OFFSET_X);
            NP[2] = CP[2] - OFFSET_Z;
        }
        case MOVE_BACK_RIGHT:{
            NP[0] = CP[0] + (-OFFSET_X + OFFSET_Y);
            NP[1] = CP[1] + (-OFFSET_Y - OFFSET_X);
            NP[2] = CP[2] - OFFSET_Z;
        }
        case MOVE_FORWARD_LEFT:{
            NP[0] = CP[0] + (OFFSET_X - OFFSET_Y);
            NP[1] = CP[1] + (OFFSET_Y + OFFSET_X);
            NP[2] = CP[2] + OFFSET_Z;
        }
        case MOVE_FORWARD_RIGHT:{
            NP[0] = CP[0] + (OFFSET_X + OFFSET_Y);
            NP[1] = CP[1] + (OFFSET_Y - OFFSET_X);
            NP[2] = CP[2] + OFFSET_Z;
        }
    }

    MoveDynamicObject(g_FlyMode[playerid][flyObject], NP[0], NP[1], NP[2], g_FlyMode[playerid][flyMovementSpeed]);

    g_FlyMode[playerid][flyTick] = GetTickCount();
}

Dialog:CameraType(playerid, response, listitem, inputtext[]){
    if(response)
    {
        new msg[128], idx;
        msg = strtok(inputtext, idx);
        if(strval(msg) == 2) g_FlyMode[playerid][flyMode] = 2; 
        else g_FlyMode[playerid][flyMode] = 1; 
    }
    AntiCommand[playerid] = 0;
    CallRemoteFunction("OnPlayerCommandText", "is", playerid, "/camera");
    return 1;
}

Dialog:diaCameraSpeed(playerid, response, listitem, inputtext[]){
    if(response)
    {
        
        new msg[128], idx;
        msg = strtok(inputtext, idx);
        new Float:speed = strval(msg);
        if(speed<=0.0 || speed>=2000.0) speed = 50.0;
        g_FlyMode[playerid][flyMovementSpeed] = speed;
    }
    AntiCommand[playerid] = 0;
    CallRemoteFunction("OnPlayerCommandText", "is", playerid, "/camera");
    return 1;
}


stock SetPlayerCustomCamera(const playerid, type = 1) {
	switch (type) {
		case 1,2: {
			TogglePlayerSpectating(playerid, true);

			new
				Float:X,
				Float:Y,
				Float:Z
			;
			GetPlayerPos(playerid, X, Y, Z);
			g_FlyMode[playerid][flyObject] = CreateDynamicObject(19300, X, Y, Z, 0.0, 0.0, 0.0, .playerid = playerid);
			AttachCameraToDynamicObject(playerid, g_FlyMode[playerid][flyObject]);

			g_FlyMode[playerid][flyType] = type;

		    g_FlyMode[playerid][flyDirection] = 0;
		    g_FlyMode[playerid][flyKeys][0] = 0;
		    g_FlyMode[playerid][flyKeys][1] = 0;
		    g_FlyMode[playerid][flyTick] = 0;
		}

	    default: {
			new Float:CP[3];
			GetPlayerCameraPos(playerid, CP[0], CP[1], CP[2]);

			TogglePlayerSpectating(playerid, false);
			SpawnPlayer(playerid);
			DestroyDynamicObject(g_FlyMode[playerid][flyObject]);
		    g_FlyMode[playerid][flyObject] = INVALID_OBJECT_ID;
			g_FlyMode[playerid][flyType] = 0;

            SetTimerEx("@delaySetPlayerPos", 100, false, "ifff", playerid,CP[0], CP[1], CP[2]);
			// SetPlayerPos(playerid, CP[0], CP[1], CP[2]);
		}
	}
}

stock SetPlayerCustomCameraSpeed(const playerid, const Float:speed) {
    g_FlyMode[playerid][flyMovementSpeed] = speed;
	return true;
}

forward @delaySetPlayerPos(const playerid,Float:x,Float:y,Float:z);
public @delaySetPlayerPos(const playerid,Float:x,Float:y,Float:z)
{
    // �⽫������ҵ�λ�ã�Ȼ����ҵ�z�����������λ���·������ʵ�ĵ��档
    SetPlayerPos(playerid, x, y, z);
    return 1;
}

Dialog:CAMERA_DIALOG_MAIN(playerid, response, listitem, inputtext[]) { //��ͷ������
    if(response) {
        switch (listitem) {
            case 0:{
                Camera_SelectDialog(playerid, PlayerCamera[playerid][Page]);
                return 1;
            }
            case 1:{
                KillTimer(PlayerCamera[playerid][cTimerID]);
                SetCameraBehindPlayer(playerid);
                return 1;
            }
            case 2:{
                Camera_ShowPlayerDialog(playerid, PlayerCamera[playerid][Page]);
                return 1;
            }
            case 3:{
                new msg[128];
                format(msg, sizeof msg, "����ǰ�ķ���ģʽΪ%d\n1:�������ƶ����ƶ��������ת�����(Ĭ��)\n2:���ֻ�������ת(�޷��ƶ�)",g_FlyMode[playerid][flyMode]);
                Dialog_Show(playerid,CameraType,DIALOG_STYLE_INPUT,"��༭����ƶ��ӽǵ�����",msg,"ȷ��","����");
                // �ӽ�����
                return 1;
            }
            case 4:{
                new msg[128];
                format(msg, sizeof msg, "����ǰ���ӽ�����Ϊ%.2f\n�����·�������Ҫ����ֵ,��ֵԽ����Խ��,��֮Խ��",g_FlyMode[playerid][flyMovementSpeed]);
                Dialog_Show(playerid,diaCameraSpeed,DIALOG_STYLE_INPUT,"��༭����ƶ��ӽǵ��ٶ�",msg,"ȷ��","����");
                // �ӽ�����
                return 1;
            }
        }
        return 1;
    }
    return 1;
}
Dialog:CAMERA_ADJUST(playerid, response, listitem, inputtext[]) { //��������
    if(response) {
        for (new i; i < CAMERA_DIALOG_ROWS; i++) {
            if(listitem == i) {
                new row;
                new title[32];
                new msg[1024];
                row = (PlayerCamera[playerid][Page] - 1) * CAMERA_DIALOG_ROWS + i;
                if(CameraInfo[playerid][CameraStatus][row] == 0) format(msg, sizeof(msg), "������ͷ", msg);
                else if(CameraInfo[playerid][CameraStatus][row] == 1) format(msg, sizeof(msg), "ɾ����ͷ\n�鿴\nMoveTime[%d]", CameraInfo[playerid][MoveTime][row]);
                format(title, sizeof(title), "��ͷ[%d]", row);
                PlayerCamera[playerid][Using] = row;
                Dialog_Show(playerid, CAMERA_INFO, DIALOG_STYLE_LIST, title, msg, "ȷ��", "ȡ��");
                return 1;
            }
        }
        switch (listitem) {
            case 10:{
                PlayerCamera[playerid][Page]--;
                Camera_ShowPlayerDialog(playerid, PlayerCamera[playerid][Page]);
                return 1;
            }
            case 11:{
                PlayerCamera[playerid][Page]++;
                Camera_ShowPlayerDialog(playerid, PlayerCamera[playerid][Page]);
                return 1;
            }
        }
        return 1;
    }
    AntiCommand[playerid] = 0;
    CallRemoteFunction("OnPlayerCommandText", "is", playerid, "/camera");
    return 1;
}
Dialog:CAMERA_INFO(playerid, response, listitem, inputtext[]) { //��ʾ��ͷ��Ϣ
    if(response) {
        new nUsing = PlayerCamera[playerid][Using];
        switch (listitem) {
            case 0:{
                if(CameraInfo[playerid][CameraStatus][nUsing] == 0) {
                    CreateCamera[playerid][CreateStatus] = 1;
                    SetPlayerCustomCamera(playerid, g_FlyMode[playerid][flyMode]);
                    SCM(playerid, Color_White, "[�����]��һ�� - ���þ�ͷλ�ú�,��������������ͷ");
                    return 1;
                }
                if(CameraInfo[playerid][CameraStatus][nUsing] == 1) {
                    DeletCamera(playerid, nUsing);
                    Camera_ShowPlayerDialog(playerid, PlayerCamera[playerid][Page]);
                    return 1;
                }
            }
            case 1:{
                SetPlayerCameraPos(playerid, CameraInfo[playerid][CameraX][nUsing], CameraInfo[playerid][CameraY][nUsing], CameraInfo[playerid][CameraZ][nUsing]);
                SetPlayerCameraLookAt(playerid, CameraInfo[playerid][CameraAtX][nUsing], CameraInfo[playerid][CameraAtY][nUsing], CameraInfo[playerid][CameraAtZ][nUsing]);
                CreateCamera[playerid][CreateStatus] = 3;
                SCM(playerid, Color_White, "[�����]������������");
                return 1;
            }
            case 2:{
                Dialog_Show(playerid, CAMERA_MoveTime, DIALOG_STYLE_INPUT, "�޸�movetime", "�����޸���ֵ", "ȷ��", "ȡ��");
                return 1;
            }
        }
        return 1;
    }
    AntiCommand[playerid] = 0;
    CallRemoteFunction("OnPlayerCommandText", "is", playerid, "/camera");
    return 1;
}
Dialog:CAMERA_MoveTime(playerid, response, listitem, inputtext[]) { //�޸�movetime����  
    if(response) {
        new id = PlayerCamera[playerid][Using];
        CameraInfo[playerid][MoveTime][id] = strval(inputtext);
        Camera_ShowPlayerDialog(playerid, PlayerCamera[playerid][Page]);
        return 1;
    }
    AntiCommand[playerid] = 0;
    CallRemoteFunction("OnPlayerCommandText", "is", playerid, "/camera");
    return 1;
}
Dialog:CAMERA_Play(playerid, response, listitem, inputtext[]) { //ѡ�񲥷ž�ͷ����
    if(response) {
        for (new i; i < CAMERA_DIALOG_ROWS; i++) {
            if(listitem == i) {
                new row;
                row = (PlayerCamera[playerid][Page] - 1) * CAMERA_DIALOG_ROWS + i;
                if(CreateCamera[playerid][CreateStatus] == 4) {
                    PlayerCamera[playerid][EndID] = row;
                    PlayerCamera[playerid][cTimerID] = SetTimerEx("PlayCamera", 200, false, "iii", playerid, PlayerCamera[playerid][StartID], PlayerCamera[playerid][EndID]);

                    if(PlayerCamera[playerid][StartID] < PlayerCamera[playerid][EndID]) {
                        PlayerCamera[playerid][TF] = 1;
                    } else {
                        PlayerCamera[playerid][TF] = 2;
                    }
                    ResetCreateInfo(playerid);
                    return 1;
                } else {
                    PlayerCamera[playerid][StartID] = row;
                    CreateCamera[playerid][CreateStatus] = 4;
                    Camera_SelectDialog(playerid, PlayerCamera[playerid][Page]);
                }
                return 1;
            }
        }
        switch (listitem) {
            case 10:{
                PlayerCamera[playerid][Page]--;
                Camera_SelectDialog(playerid, PlayerCamera[playerid][Page]);
                return 1;
            }
            case 11:{
                PlayerCamera[playerid][Page]++;
                Camera_SelectDialog(playerid, PlayerCamera[playerid][Page]);
                return 1;
            }
        }
        return 1;
    }
    AntiCommand[playerid] = 0;
    CallRemoteFunction("OnPlayerCommandText", "is", playerid, "/camera");
    return 1;
}