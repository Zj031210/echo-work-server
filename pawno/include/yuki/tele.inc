// IORP Script
/*
    This server is modified from 5F free residential open source, Prace open source
    To the makers of Prace:[Ghost]Rui ushio_p [Ghost]Dylan
    Tribute to the makers of the free settlement :[ITC]dyq [ITC]fangye [ITC]Super_wlc [ITC
    Special thanks to ryddawn and technical advisor [Fire]KiVen OBJ:JoshenKM;

								The MIT License  
								
	Copyright (c) <2019-2020> <YuCarl77>  
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
	��Ȩ(c) <2019-2020> <YuCarl77>   

	ʹ�ø�����֤����������������Ȩ�ޣ���ѣ��κ��˿��Եõ����������������ĵ���һ��������
	���Ҿ�Ӫ�����������κ����ƣ����������Ƶ�ʹ�á����ơ��޸ġ��ϲ������桢���С����Ŵ���֤�顢���߳��۸������Ŀ�����Ȩ����
	ͬʱ���������Щ�������û�������ЩȨ����ʹ����������������  
	
	���ϵİ�Ȩ֪ͨ��Ȩ��֪ͨӦ�ð��������и������Ŀ����л�������������������ʵ�����С�
	
	�������������������ṩ��û���κ���ʽ�ĵ�������������ȷ�ػ��߰����ģ�������Щ���ǲ�����ҵ���ʵĵ��������ơ�
	�ʺ�һ���ض�����;���Ҳ����ַ������ߺͰ�Ȩ���������κγ��϶�ʹ�ø������漰���κ�Ҫ���𺦻����������ζ���Ӧ����
	�����������������û���ֻ�Ǻ�ͬ��ʽ��������Ȩ����������ʽ�������������������÷�Χ�ڡ������������ϵ����������ʹ�û�����������������������Ϊ��  
	=====================================   
*/

//  �����ң����ڿ�Դ��д��лǰ�����˵����������֮���١�
//  �����޸���5F��Prace,PHouse,Goods�ȿ�Դ �ر���л��[Ghost]Rui ushio_p [Ghost]Dylan
//  �¾����ɾ�������Դ,������:[ITC]dyq  [ITC]fangye  [ITC]Super_wlc [ITC]RR_LXD  mk124  Shindo(aka. ssh)  vvg, yezizhu(aka. yzz)
//  �ر���л ryddawn ������OBJָ����[Fire]KiVen JoshenKM
//  �ر���л GoodsSys Episodes KiVen juse
//  �ر���л ��Щʼ��֧�ֲ�ʹ��Shoebill���ˡ�  
//  �ر���л GTAUN GTABBS 
// RST�Ŷӷ�����������Ҫ��[R_ST]Hygen��YuCarl77���쵼

#define MAX_TRANSFER        	500//����͵�500�� //Ĭ��10000��
// #define TeleListDialog			714 //700�ǹ���Ƶ� ռ��һ��714
stock GetPlayerFacingAngleEx(const playerid, & Float:ang) {
    if(GetPlayerVehicleID(playerid) == 0) {
        GetPlayerFacingAngle(playerid, ang);
    } else {
        GetVehicleZAngle(GetPlayerVehicleID(playerid), ang);
    }
}
enum maketype {
    mbuff,
    Float:mx,
    Float:my,
    Float:mz,
    Float:ma,
    mname[128]
};
enum makesystype {
    mbuff,
    Float:mx,
    Float:my,
    Float:mz,
    Float:ma,
    mname[128],
    tdescribe[48]
};
new vmake[MAX_TRANSFER][maketype], TransferColor = 0xFFFF00FF;
new vsysmake[MAX_TRANSFER][makesystype];
new TelePage[MAX_PLAYERS];
stock pVmakePos(const playerid, const id, const text[]) {
    new str[128];
    format(str, sizeof(str), "[����] �㴴����һ��'//%s'�Ĵ��͵�", text);
    SendClientMessage(playerid, TransferColor, str);
    format(vmake[id][mname], 128, "%s", text);
    vmake[id][mbuff] = 1;
    GetPlayerPos(playerid, vmake[id][mx], vmake[id][my], vmake[id][mz]);
    GetPlayerFacingAngleEx(playerid, vmake[id][ma]);
    new File:m = fopen("Teleport/Transfer.pos", io_append), info[1024];
    format(info, 1024, "%s %f %f %f %f\r\n", vmake[id][mname], vmake[id][mx], vmake[id][my], vmake[id][mz], vmake[id][ma]);
    make_fwrite(m, info);
    fclose(m);
    return 1;
}
stock pVmakeSysPos(const playerid, const id, const text[], const describe[]) {
    new str[128];
    format(str, sizeof(str), "[����] �㴴����һ��'/%s'��ϵͳ���͵�", text);
    SendClientMessage(playerid, TransferColor, str);
    format(vsysmake[id][mname], 128, "%s", text);
    vsysmake[id][mbuff] = 1;
    GetPlayerPos(playerid, vsysmake[id][mx], vsysmake[id][my], vsysmake[id][mz]);
    GetPlayerFacingAngleEx(playerid, vsysmake[id][ma]);
    format(vsysmake[id][tdescribe], 48, "%s", describe);
    new File:m = fopen("Teleport/SysTransfer.pos", io_append), info[1024];
    format(info, 1024, "%s %f %f %f %f %s\r\n", vsysmake[id][mname], vsysmake[id][mx], vsysmake[id][my], vsysmake[id][mz], vsysmake[id][ma], vsysmake[id][tdescribe]);
    make_fwrite(m, info); //�������ڰ�\r\n�������
    fclose(m);
    format(str, sizeof(str), "������λ��%s ָ��/%s\n����%f,%f,%f", vsysmake[id][tdescribe], vsysmake[id][mname], vsysmake[id][mx], vsysmake[id][my], vsysmake[id][mz]);
    CreateDynamic3DTextLabel(str, 0x2BA2D5C8, vsysmake[id][mx], vsysmake[id][my], vsysmake[id][mz], 30.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, 0, -1, -1, STREAMER_3D_TEXT_LABEL_SD, -1, 0);
    return 1;
}
stock Initialize_Transfer() {
    print("[��ʾ]������Ҵ���");
    if(fexist("Teleport/Transfer.pos") == 1) {
        new File:mk = fopen("Teleport/Transfer.pos", io_read), mid, makeinfo[1024], name[128], idx = 0, loads = 0;
        while (fread(mk, makeinfo)) {
            format(name, 128, "%s", strtok(makeinfo, idx));
            mid = make_getindex();
            if(mid == -1) {
                printf("[��ʾ]������ƴ��͵�����[%s]����,û�п���makeid.", name);
            } else {
                format(vmake[mid][mname], 128, "%s", name);
                vmake[mid][mx] = floatstr(strtok(makeinfo, idx));
                vmake[mid][my] = floatstr(strtok(makeinfo, idx));
                vmake[mid][mz] = floatstr(strtok(makeinfo, idx));
                vmake[mid][ma] = floatstr(strtok(makeinfo, idx));
                vmake[mid][mbuff] = 1;
                loads++;
            }
            idx = 0;
        }
        fclose(mk);
        printf("[��ʾ]��ȡ����[%d]��,ʣ����ô���[%d]��", loads, MAX_TRANSFER - loads);
    }
    return 1;
}
stock Initialize_SysTransfer() {
    print("[��ʾ]����ϵͳ����");
    if(fexist("Teleport/SysTransfer.pos") == 1) {
        new File:mk = fopen("Teleport/SysTransfer.pos", io_read), mid, makeinfo[1024], name[128], idx = 0, loads = 0;
        while (fread(mk, makeinfo)) {
            format(name, 128, "%s", strtok(makeinfo, idx));
            mid = make_sysgetindex();
            if(mid == -1) {
                printf("[����]ϵͳ���ƴ��͵�����[%s]����,�Ѵ�����. ", name);
            } else {
                format(vsysmake[mid][mname], 128, "%s", name);
                vsysmake[mid][mx] = floatstr(strtok(makeinfo, idx));
                vsysmake[mid][my] = floatstr(strtok(makeinfo, idx));
                vsysmake[mid][mz] = floatstr(strtok(makeinfo, idx));
                vsysmake[mid][ma] = floatstr(strtok(makeinfo, idx));
                //������ͼ�ϵ�3D��ʾ
                format(vsysmake[mid][tdescribe], 48, "%s", strtok(makeinfo, idx));
                vsysmake[mid][mbuff] = 1;
                loads++;
                new str[128] = "";
                //2020.2.4���� ������ ����ID0 ��ʾ3D����
                format(str, sizeof(str), "������λ��%s ָ��/%s\n����%f,%f,%f", vsysmake[mid][tdescribe], vsysmake[mid][mname], vsysmake[mid][mx], vsysmake[mid][my], vsysmake[mid][mz]);
                // Create3DTextLabel(str, 0x1EA7E1C8, vsysmake[mid][mx], vsysmake[mid][my], vsysmake[mid][mz], 40.0, 0, 0);
                //�������ⴴ��3D����
                CreateDynamic3DTextLabel(str, 0x2BA2D5C8, vsysmake[mid][mx], vsysmake[mid][my], vsysmake[mid][mz], 30.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, 0, -1, -1, STREAMER_3D_TEXT_LABEL_SD, -1, 0);
            }
            idx = 0;
        }
        fclose(mk);
        printf("[��ʾ]��ȡϵͳ����[%d]��,ʣ����ô���[%d]�� <<<<<<", loads, MAX_TRANSFER - loads);
    }
    return 1;
}
stock make_fwrite(File:handle, const text[]) {
    new l = strlen(text);
    for (new i = 0; i < l; i++) {
        fputchar(handle, text[i], false);
    }
}
stock make_getindex() {
    new s = -1;
    for (new i = 0; i < MAX_TRANSFER; i++) {
        if(vmake[i][mbuff] == 0) {
            s = i;
            i = MAX_TRANSFER + 1;
        }
    }
    return s;
}
stock make_sysgetindex() {
    new s = -1;
    for (new i = 0; i < MAX_TRANSFER; i++) {
        if(vsysmake[i][mbuff] == 0) {
            s = i;
            i = MAX_TRANSFER + 1;
        }
    }
    return s;
}
stock make_findgo(const name[]) {
    new s = -1;
    for (new i = 0; i < MAX_TRANSFER; i++) {
        if(vmake[i][mbuff] == 1) {
            if(strcmp(vmake[i][mname], name, true) == 0) {
                s = i;
                i = MAX_TRANSFER + 1;
            }
        }
    }
    return s;
}
stock make_Sysfindgo(const name[]) {
    new s = -1;
    for (new i = 0; i < MAX_TRANSFER; i++) {
        if(vsysmake[i][mbuff] == 1) {
            if(strcmp(vsysmake[i][mname], name, true) == 0) {
                s = i;
                i = MAX_TRANSFER + 1;
            }
        }
    }
    return s;
}

stock Tele_OnPlayerLogin(const playerid){
	TelePage[playerid] = 1;
}

stock Tele_ShowListDialog(const playerid, const page) //Ϊָ�������ʾ����ѡ��˵�,page��ʾ����ҳ��
{
    new title[64], msg[1024], maxpage;
    maxpage = make_sysgetindex() / 10;
	if(maxpage % 10 != 0) maxpage++;
    format(title, 64, "ϵͳ���� - %i/%i", page, maxpage);
    format(msg, sizeof(msg), "���͵�\t����\tID\n", msg);
    if(page > 1 && page == maxpage) format(msg, sizeof(msg), "%s\n��һҳ", msg);
    if(page == 1 && page != maxpage) format(msg, sizeof(msg), "%s\n��һҳ\n", msg);
    if(page > 1 && page < maxpage) format(msg, sizeof(msg), "%s\n��һҳ\n��һҳ", msg);
    for (new i = (page - 1) * 10; i < page * 10; i++) {
        if(vsysmake[i][mbuff] == 1) { //������ڵĻ�
    	format(msg, sizeof(msg), "%s\n%s\t%s\t%d\n", msg, vsysmake[i][mname], vsysmake[i][tdescribe], i + 1);
        }
    }
    Dialog_Show(playerid, TeleListDialog, DIALOG_STYLE_TABLIST_HEADERS, title, msg, "����", "�ر�");
    return 1;
}

Dialog:TeleListDialog(playerid, response, listitem, inputtext[]) {
    if(response == 1) {
        new msg[128], idx;
        msg = strtok(inputtext, idx);
        if(strcmp(msg, "��һҳ") == 0) {
            TelePage[playerid]--;
            Tele_ShowListDialog(playerid, TelePage[playerid]);
            return 1;
        }
        if(strcmp(msg, "��һҳ") == 0) {
            TelePage[playerid]++;
            Tele_ShowListDialog(playerid, TelePage[playerid]);
            return 1;
        }
        format(msg, sizeof msg, "/%s", inputtext);
        AntiCommand[playerid] = 0;
        CallRemoteFunction("OnPlayerCommandText", "is", playerid, msg);
        // OnPlayerCommandText(playerid, msg);
        return 1;
    }
    return 1;
}