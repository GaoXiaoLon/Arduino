/*
 Name:		TCJ_Communication.ino
 Created:	2021/7/29 10:09:08
 Author:	Gao
*/

//�����Լ���ʵ�ʸ�Ϊ��Ӧ�Ĵ���
#define TJC Serial3
//�������ݳ���
#define FRAME_LENGTH 6

//���ͽ�����
void SendEnd() {
    TJC.write(0xff);
    TJC.write(0xff);
    TJC.write(0xff);
}


unsigned long nowtime;
void setup() {
    // put your setup code here, to run once:
    TJC.begin(115200);
    nowtime = millis(); //��ȡ��ǰ�Ѿ����е�ʱ��
}


int a = 0;
void loop() {
    // put your main code here, to run repeatedly:
    char str[100];
    //ʵ��ÿ��1S����1��
    if (millis() >= nowtime + 50000)
    {
        nowtime = millis(); //��ȡ��ǰ�Ѿ����е�ʱ��

        //��sprintf����ʽ���ַ�������n0��val���Ը�ֵ
        sprintf(str, "n0.val=%d", a);
        //���ַ������ͳ�ȥ
        TJC.print(str);
        //���ͽ�����
        SendEnd();

        //��sprintf����ʽ���ַ�������t0��txt���Ը�ֵ
        sprintf(str, "t0.txt=\"������%d\"", a);
        //���ַ������ͳ�ȥ
        TJC.print(str);
        //���ͽ�����
        SendEnd();

        //��sprintf����ʽ���ַ���������b0�İ����¼�
        sprintf(str, "click b0,1");
        //���ַ������ͳ�ȥ
        TJC.print(str);
        //���ͽ�����
        SendEnd();

        //��sprintf����ʽ���ַ���������b0�ĵ����¼�    
        sprintf(str, "click b0,0");
        //���ַ������ͳ�ȥ    
        TJC.print(str);
        //���ͽ�����    
        SendEnd();

        a++;
    }


    //�������ݸ�ʽ��
    //��������֡���ȣ�6�ֽ�
    //��ͷ      led���  LED״̬    ��β
    //0x55      1�ֽ�    1�ֽ�     0xffffff
    //����1����λ������  printh 55 01 00 ff ff ff  ���壺1��led�ر�
    //����2����λ������  printh 55 04 01 ff ff ff  ���壺4��led��
    //����3����λ������  printh 55 00 01 ff ff ff  ���壺0��led��
    //����4����λ������  printh 55 09 00 ff ff ff  ���壺4��led�ر�

    //�����ڻ��������ڵ���6ʱ
    if (TJC.available() >= FRAME_LENGTH) {
        unsigned char ubuffer[FRAME_LENGTH];
        //�ӻ�������ȡ6���ֽ�ֱ������0xffffff  TJC.readBytesUntil
        int ret = Serial3.readBytesUntil(0xffffff, ubuffer, FRAME_LENGTH);
        //����ȡ������6���ֽ�ʱ
        if (ret == FRAME_LENGTH) {
            //�ж�ubuffer[0]�Ƿ�Ϊ0x55��֡β�Ƿ�Ϊ0xffffff
            if (ubuffer[0] == 0x55 && ubuffer[3] == 0xff && ubuffer[4] == 0xff && ubuffer[5] == 0xff) {
                char str2[100];
                sprintf(str2, "t0.txt=\"led %d is %s\"", ubuffer[1], ubuffer[2] ? "on" : "off");
                TJC.print(str2);
                SendEnd();
            }
        }
    }

}
