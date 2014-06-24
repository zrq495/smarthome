#include "cliect_td.h"
NodeInfo *pNodeInfo2;
NwkDesp pNwkDesp2;
NodeInfo *arr[20]={NULL,};
int i_depth=0;
int flag_coord=0;
unsigned long int ptemperature;
unsigned int ptemp;
unsigned int phum;
unsigned long int pid;
unsigned long int id_temp=0;
static int irda_warn_flag=0,irda_warn_flag_temp=0;
static int int_warn_flag=0,int_warn_flag_temp=0;
static int smog_warn_flag=0,smog_warn_flag_temp=0;
int sockfd;
Cliect::Cliect()
{
    coorstate=0;
}

Cliect::~Cliect()
{
}


void Cliect::run(){
    unsigned int buffer[363]={0};
    while(1)
    {


        unsigned int temp;
        int i=0,count=0;
        tv.tv_sec=2;
        tv.tv_usec=0;
        FD_ZERO(&sds);
        FD_SET(sockfd, &sds);
        int ret = select((1+sockfd), &sds, NULL, NULL, &tv);
        if(ret >0)
        {
            //printf("server select wait...\n");
            if (FD_ISSET(sockfd, &sds))
            {
                //printf("cliect FD_ISSET...\n");
                int len=0;
                len=::recv(sockfd,&buffer[i],sizeof(unsigned int),0);
                if(len>0)
                {

                   // printf("cliect buffer[%d]=%d\n",i,buffer[i]);
                while(buffer[i]!=0x0A)
                {
                   i++;
                   len=::recv(sockfd,&buffer[i],sizeof(unsigned int),0);
                   //printf("cliect buffer[%d]=%d\n",i,buffer[i]);
                }
                if(buffer[0]==0x26)
                {
                    temp=buffer[1];
                    count=i+1;
                    switch(temp)
                    {
                    case 0x01:

                            //printf("COMMAND:-------TOPOINFO--------:\n");
                            Cliect_ZigBeeNwkTopo_Process(&buffer[2],count);
                            break;
                    case 0x02:

                            //printf("COMMAND:-------GetZigBeeNwkInfo--------:\n");
                            Cliect_ZigBeeNwkInfo_Process(&buffer[2]);
                            break;

                    case 0x04:

                            //printf("COMMAND:-------GetRfidId--------:\n");
                            Cliect_RfidId_Process(&buffer[2]);
                            break;
                    case 0x05:

                            //printf("COMMAND:-------GetTempHum--------:\n");
                            Cliect_TempHum_Process(&buffer[2]);
                            break;
                    case 0x06:

                            //printf("COMMAND:-------GetSendMsg--------:\n");
                            Cliect_GPRSSend_Process(buffer[2]);
                            break;

                    case 0x07:

                            printf("COMMAND:-------get GPRSSignal--------:\n");
                            Cliect_GPRSSignal_Process(buffer[2]);

                            break;

                    default:
                            printf("error COMMAND\n");
                            break;

                    }
                }
                else
                {
                    printf("other protrol.\n");
                }
            }



            }
            else{
                               printf("not  cliect fd.\n");
                    }
        }
        else if(ret == 0){
            printf("cliect read wait timeout!!!\n");

        }
        else{// ret <0
            printf("cliect select error.\n");
            //perror(ret);
        }
        usleep(30000);

    }

}
void Cliect::GetConnect(char *ipaddr,int port){
    struct sockaddr_in servaddr;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    qDebug("GetConnect sockfd:%d\n",sockfd);
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");


    servaddr.sin_port = htons(port);

    if(::connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr))!=0){
    perror("Connect ");
    //exit(errno);
    }

}
void Cliect::Cliect_ZigBeeNwkTopo_Process(unsigned int *node,unsigned int count)
{

    if(count<5)
    {
        if((*node)==0x01)
        {
            coorstate=1;
        }//gxj
        else if((*node)==0x02)
        {
            coorstate=2;
        }//gxj
        else
            coorstate=0;



    }
    else
    {
        coorstate=0;
        int i=0;
        NodeInfo* pNodeNwkTopoInfo,*q,*p;
        pNodeNwkTopoInfo=(struct NodeInfo *) malloc(sizeof(struct NodeInfo));
        pNodeNwkTopoInfo->devinfo=(struct DeviceInfo*)malloc(sizeof(struct DeviceInfo));
        pNodeNwkTopoInfo->devinfo->nwkaddr=(*node);
        pNodeNwkTopoInfo->devinfo->macaddr[0]=*(node+1);
        pNodeNwkTopoInfo->devinfo->macaddr[1]=*(node+2);
        pNodeNwkTopoInfo->devinfo->macaddr[2]=*(node+3);
        pNodeNwkTopoInfo->devinfo->macaddr[3]=*(node+4);
        pNodeNwkTopoInfo->devinfo->macaddr[4]=*(node+5);
        pNodeNwkTopoInfo->devinfo->macaddr[5]=*(node+6);
        pNodeNwkTopoInfo->devinfo->macaddr[6]=*(node+7);
        pNodeNwkTopoInfo->devinfo->macaddr[7]=*(node+8);
        pNodeNwkTopoInfo->devinfo->depth=*(node+9);
        pNodeNwkTopoInfo->devinfo->devtype=*(node+10);
        pNodeNwkTopoInfo->devinfo->parentnwkaddr=*(node+11);

        pNodeNwkTopoInfo->devinfo->sensortype=*(node+12);
        pNodeNwkTopoInfo->devinfo->sensorvalue=(*(node+13));//+((*(node+14))<<16);
        pNodeNwkTopoInfo->devinfo->status=*(node+15);
        pNodeNwkTopoInfo->row=*(node+16);
        pNodeNwkTopoInfo->num=*(node+17);
        pNodeNwkTopoInfo->next=NULL;

        p=pNodeNwkTopoInfo;
        qDebug("topo i=%d,count=%d\n",i,count);

        for(i=1;i<(count-3)/18;i++)
        {

            //qDebug("topo i=%d\n",i);
            q=(struct NodeInfo *) malloc(sizeof(struct NodeInfo));
            q->devinfo=(struct DeviceInfo*)malloc(sizeof(struct DeviceInfo));
            q->devinfo->nwkaddr=*(node+18*i);
            q->devinfo->macaddr[0]=*(node+1+18*i);
            q->devinfo->macaddr[1]=*(node+2+18*i);
            q->devinfo->macaddr[2]=*(node+3+18*i);
            q->devinfo->macaddr[3]=*(node+4+18*i);
            q->devinfo->macaddr[4]=*(node+5+18*i);
            q->devinfo->macaddr[5]=*(node+6+18*i);
            q->devinfo->macaddr[6]=*(node+7+18*i);
            q->devinfo->macaddr[7]=*(node+8+18*i);
            q->devinfo->depth=*(node+9+18*i);
            q->devinfo->devtype=*(node+10+18*i);
            q->devinfo->parentnwkaddr=*(node+11+18*i);

            q->devinfo->sensortype=*(node+12+18*i);
            q->devinfo->sensorvalue=(*(node+13+18*i));//+((*(node+14+18*i))<<16);
            q->devinfo->status=*(node+15+18*i);
            q->row=*(node+16+18*i);
            q->num=*(node+17+18*i);
            q->next=NULL;
            pNodeNwkTopoInfo->next=q;
            pNodeNwkTopoInfo=pNodeNwkTopoInfo->next;
        }
        pNodeNwkTopoInfo->next=NULL;
        pNodeNwkTopoInfo=p;
        pNodeInfo2=pNodeNwkTopoInfo;



    }
    Layer(coorstate);


}
void Cliect::Cliect_ZigBeeNwkInfo_Process(unsigned int *nwkinfo)
{
    pNwkDesp2.panid=*nwkinfo;
    pNwkDesp2.channel=*(nwkinfo+1);
    //qDebug("pNwkDesp2.channel=%ld\n",pNwkDesp2.channel);
    //qDebug("pNwkDesp2->channel=%d %d\n",*(nwkinfo+2),(*(nwkinfo+1)));
    pNwkDesp2.maxchild=*(nwkinfo+3);
    pNwkDesp2.maxdepth=*(nwkinfo+4);
    pNwkDesp2.maxrouter=*(nwkinfo+5);

}
void Cliect::Cliect_RfidId_Process(unsigned int *id)
{
    pid=*(id);//+((*(id+1))<<16);
    if(pid!=id_temp)
            {
                id_temp=pid;
                emit Rfid_Changed();
            }
}
void Cliect::Cliect_TempHum_Process(unsigned int *temp)
{
    ptemperature=*(temp);//+((*(temp+1))<<16);

    qDebug("temp=%d hum=%d\n",*(temp),((*(temp+1)))<<16);

}
 void Cliect::Cliect_GPRSSignal_Process(unsigned int sig)
 {
    qDebug("....................success!.......................\n");
        emit Sim(sig);
    //gprssig=sig;
 }
 void Cliect::Cliect_GPRSSend_Process(unsigned int sig)
 {
    qDebug("---------------------send....-----------------------\n ");
        emit Send(sig);
    //gprssig=sig;
 }
void Cliect::Layer(int coorstate)
{
    int nodechange_state=0;
    NodeInfo *p=pNodeInfo2,*q,*t;
    if((coorstate==1)||(coorstate==2))
    {
        if(coorstate==1)
            flag_coord=0;
        else if(coorstate==2)
            flag_coord=1;
        int i=1;
        while(arr[i]!=NULL)
        {
            NodeInfo *p=arr[i];
            NodeInfo *q,*t=p;
            while(p!=NULL)
            {
                q=p;
                p=p->next;
                qDebug("delete nwkaddr:%d\n",q->devinfo->nwkaddr);
                delete q;

            }
            arr[i]=NULL;            i++;
        }
        t=pNodeInfo2;
        while(pNodeInfo2!=NULL)
        {
            q=pNodeInfo2;
            pNodeInfo2=pNodeInfo2->next;
            //qDebug("delete:%0x\n",q->devinfo->nwkaddr);
            delete q;
        }
        q=pNodeInfo2=t;
        pNodeInfo2=NULL;
        i_depth=0;
        //emit StateChanged();
        nodechange_state=1;
    }
    else
    {
        flag_coord=1;
        int flag_nwkaddr=0;
        while(p)
        {
            qDebug("%0x(parentnwkaddr:%0x,depth:%0x,status:%0x)\n",p->devinfo->nwkaddr,p->devinfo->parentnwkaddr,p->devinfo->depth,p->devinfo->status);


        if(p->devinfo->status)
        {
            if(p->devinfo->sensortype==0x1)
            {
                irda_warn_flag=p->devinfo->sensorvalue;
                Api_Cliect_ClearIntlock();

                if(irda_warn_flag!=irda_warn_flag_temp)
                {

                    if(irda_warn_flag==0x1)
                    {

                        Irda_StateChanged_1();
                        qDebug("--------------------------------Irda_StateChanged_1---------------------");
                        sendMsgsignal(1);
                        }
                    else
                    {

                        Irda_StateChanged_0();
                        qDebug("--------------------------------Irda_StateChanged_0---------------------");
                        }

                }
                irda_warn_flag_temp=irda_warn_flag;

            }


            if(p->devinfo->sensortype==0x2)
            {
                smog_warn_flag=p->devinfo->sensorvalue;
                Api_Cliect_ClearIntlock();
                if(smog_warn_flag!=smog_warn_flag_temp)
                {

                    if(smog_warn_flag==0x1)
                    {

                        Smog_StateChanged_1();
                        sendMsgsignal(2);
                        qDebug("--------------------------------Smog_StateChanged_1---------------------");
                    }
                    else
                    {
                        //Api_Cliect_ClearIntlock();
                        Smog_StateChanged_0();
                        qDebug("--------------------------------Smog_StateChanged_0---------------------");
                     }

                }
                smog_warn_flag_temp=smog_warn_flag;
                }


            if(p->devinfo->sensortype==0x3)
            {
                int_warn_flag=p->devinfo->sensorvalue;
                Api_Cliect_ClearIntlock();
                //qDebug("int_warn_flag=%d,int_warn_flag_temp=%d",int_warn_flag,int_warn_flag_temp);
                if(int_warn_flag!=int_warn_flag_temp)
                {

                    if(int_warn_flag==0x1)
                    {

                        Int_StateChanged_1();
                        qDebug("--------------------------------Int_StateChanged_1---------------------");
                        sendMsgsignal(3);
                    }
                    else
                    {
                        Api_Cliect_ClearIntlock();
                        qDebug("--------------------------------Int_StateChanged_0---------------------");
                        Int_StateChanged_0();
                    }


                }
                int_warn_flag_temp=int_warn_flag;

            }

            if(p->devinfo->sensortype==0x0)
            if(p->devinfo->sensorvalue!=0)
            sendTempHum(p->devinfo->sensorvalue);

        }




        if(p->devinfo->depth>i_depth)
        i_depth=p->devinfo->depth;
        t=arr[p->devinfo->depth];
        flag_nwkaddr=0;
        while(t)
        {

            if(p->devinfo->nwkaddr==t->devinfo->nwkaddr)
            {
                //qDebug("%d status:%d, %dstatus:%d,nodechange_state=%d\n",p->devinfo->nwkaddr,p->devinfo->status,t->devinfo->nwkaddr,t->devinfo->status,nodechange_state);
                if(p->devinfo->status!=t->devinfo->status)
                {
                        t->devinfo->status=p->devinfo->status;
                        nodechange_state=1;
                        //qDebug("%d status !=%d,nodechange_state=%d\n",p->devinfo->nwkaddr,t->devinfo->nwkaddr,nodechange_state);
                }
                flag_nwkaddr=1;
                break;

            }
            t=t->next;
        }
        {
            if(i_depth<=p->devinfo->depth)
            {
                int j=1;
                for(j=1;j<p->devinfo->depth;j++)
                {
                    t=arr[j];
                    while(t)
                    {

                        if(p->devinfo->nwkaddr==t->devinfo->nwkaddr)
                        {

                            t->devinfo->status=0;
                            nodechange_state=1;
                            qDebug("the depth%d,alread have%d\n",t->devinfo->depth,p->devinfo->nwkaddr);
                            break;
                        }
                        t=t->next;
                    }

                }

            }
            else if(i_depth>p->devinfo->depth)
            {
                int j=1;
                for(j=1;j<p->devinfo->depth;j++)
                {
                    t=arr[j];
                    while(t)
                    {

                        if(p->devinfo->nwkaddr==t->devinfo->nwkaddr)
                        {

                            t->devinfo->status=0;
                            nodechange_state=1;
                            qDebug("the depth%d,alread have%d\n",t->devinfo->depth,p->devinfo->nwkaddr);

                            break;
                        }
                        t=t->next;
                    }

                }
                for(j=p->devinfo->depth+1;j<i_depth+1;j++)
                {
                    t=arr[j];
                    while(t)
                    {

                        if(p->devinfo->nwkaddr==t->devinfo->nwkaddr)
                        {

                            t->devinfo->status=0;
                            nodechange_state=1;
                            qDebug("the depth%d,alread have%d\n",t->devinfo->depth,p->devinfo->nwkaddr);

                            break;
                        }
                        t=t->next;
                    }

                }

            }
        }
        if(!flag_nwkaddr)
        {
            q=p->next;
            p->next=arr[p->devinfo->depth];
            arr[p->devinfo->depth]=p;
            p=q;
            //emit StateChanged();
            nodechange_state=1;
            flag_nwkaddr=0;    

        }
        else
        {   //qDebug("flag_nwkaddr\n");
            p=p->next;
            flag_nwkaddr=0;
        }


        }//while
    }

    if(nodechange_state)
    {
        emit StateChanged();
    }
}


void Cliect::Api_Cliect_ClearIntlock()
{
    unsigned int buffer[3]={0};
    buffer[0]=0x15;
    buffer[1]=0x08;

    buffer[2]='\n';

    int len=::write(sockfd, (unsigned int*)(&buffer), sizeof(buffer));
    if(len<=0)
    {

        qDebug("Api_Cliect_ClearIntlock send error\n");
    }
}
void Cliect::Api_Cliect_GetZigBeeNwkInfo()
{
    //qDebug("Api_Cliect_GetZigBeeNwkInfo send \n");
    unsigned int buffer[3]={0};
    buffer[0]=0x15;
    buffer[1]=0x02;

    buffer[2]='\n';

    int len=::write(sockfd, (unsigned int*)(&buffer), sizeof(buffer));
    if(len<=0)
    {

        qDebug("Api_Cliect_GetZigBeeNwkInfo send error\n");
    }
}
void Cliect::Api_Cliect_GetZigBeeNwkTopo()
{
    //qDebug("Api_Cliect_GetZigBeeNwkTopo send \n");
    unsigned int buffer[3]={0};
    buffer[0]=0x15;
    buffer[1]=0x01;

    buffer[2]='\n';
    //qDebug("Api_Cliect_GetZigBeeNwkInfo send sockfd:%d\n",sockfd);
    int len=::write(sockfd, (unsigned int*)(&buffer), sizeof(buffer));
    if(len<=0)
    {

        qDebug("Api_Cliect_GetZigBeeNwkTopo send error\n");
    }
}
void Cliect::Api_Cliect_GetTempHum()
{
    //qDebug("Api_Cliect_GetTempHum send \n");
    unsigned int buffer[3]={0};
    buffer[0]=0x15;
    buffer[1]=0x05;

    buffer[2]='\n';
    //qDebug("Api_Cliect_GetTempHum send sockfd:%d\n",sockfd);
    int len=::write(sockfd, (unsigned int*)(&buffer), sizeof(buffer));
    if(len<=0)
    {

        qDebug("Api_Cliect_GetTempHum send error\n");
    }
}
void Cliect::Api_Cliect_GetRfidId()
{
    //qDebug("Api_Cliect_GetRfidId send \n");
    unsigned int buffer[3]={0};
    buffer[0]=0x15;
    buffer[1]=0x04;

    buffer[2]='\n';
    //qDebug("Api_Cliect_GetRfidId send sockfd:%d\n",sockfd);
    int len=::write(sockfd, (unsigned int*)(&buffer), sizeof(buffer));
    if(len<=0)
    {

        qDebug("Api_Cliect_GetRfidId send error\n");
    }
}
void Cliect::Api_Cliect_GetGPRSSignal()
{
    //qDebug("Api_Cliect_GetGPRSSignal send \n");
    unsigned int buffer[3]={0};
    buffer[0]=0x15;
    buffer[1]=0x07;

    buffer[2]='\n';
    //qDebug("Api_Cliect_GetZigBeeNwkInfo send sockfd:%d\n",sockfd);
    int len=::write(sockfd, (unsigned int*)(&buffer), sizeof(buffer));
    if(len<=0)
    {

        qDebug("Api_Cliect_GetGPRSSignal send error\n");
    }
}
void Cliect::Api_Cliect_SendGprsMessage(char *phone,int sensor)
{
    //qDebug("Api_Cliect_SendGprsMessage send \n");
    unsigned int buffer[15]={0};
    buffer[0]=0x15;
    buffer[1]=0x06;
    int i=0;
    qDebug("Api_Cliect_SendGprsMessage send :\n");
    for(i=0;i<11;i++)
    {
        buffer[2+i]=*(phone+i);
        qDebug("%d",*(phone+i));
    }
    qDebug("\n");
    buffer[13]=sensor;
    buffer[14]='\n';
    //qDebug("Api_Cliect_GetZigBeeNwkInfo send sockfd:%d\n",sockfd);
    int len=::write(sockfd, (unsigned int*)(&buffer), sizeof(buffer));
    if(len<=0)
    {

        qDebug("Api_Cliect_SendGprsMessage send error\n");
    }
}

void Cliect::Api_Cliect_SetSensorStatus(unsigned int nwkaddr,unsigned int Mode)
{
    qDebug("Api_Cliect_SetSensorStatus send \n");
    unsigned int buffer[5]={0};
    buffer[0]=0x15;
    buffer[1]=0x03;

    buffer[2]=nwkaddr;
    buffer[3]=Mode;
    buffer[4]='\n';
    qDebug("Api_Cliect_GetZigBeeNwkInfo send sockfd:%d\n",sockfd);
    int len=::write(sockfd, (unsigned int*)(&buffer), sizeof(buffer));
    if(len<=0)
    {

        qDebug("Api_Cliect_SetSensorStatus send error\n");
    }
}
