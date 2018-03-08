#include <ros/ros.h>
#include <actionlib/client/simple_action_client.h>
#include <actionlib/client/terminal_state.h>
#include <control_msgs/JointTrajectoryAction.h>
#include <moveit_msgs/MoveGroupActionGoal.h>
#include <moveit_msgs/MoveGroupAction.h>
#include <moveit_msgs/GetPositionIK.h>
#include <moveit_msgs/KinematicSolverInfo.h>
#include <moveit_msgs/PositionIKRequest.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <dirent.h>
#include <linux/input.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/select.h>
#include <sys/time.h>
#include <termios.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <termios.h>
#include <termios.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <arpa/inet.h>
#include "../../../../../home/tono/Plocha/OdbornaPrax/odborna prax/m2dmini.h"
#include <bitset>
#include <string>

char buff_packet[10000];
int vektorDistance[1000];
int poradie=0;;

void error(const char *msg){
    perror(msg);
    exit(0);
}

void parse_data();
std::string toBinary(int);
long toDecimal(std::string binary1);

int getch(void) {
    int c=0;

    struct termios org_opts, new_opts;
    int res=0;
    //-----  store old settings -----------
    res=tcgetattr(STDIN_FILENO, &org_opts);
    assert(res==0);
    //---- set new terminal parms --------
    memcpy(&new_opts, &org_opts, sizeof(new_opts));
    new_opts.c_lflag &= ~(ICANON | ECHO | ECHOE | ECHOK | ECHONL | ECHOPRT | ECHOKE | ICRNL);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_opts);
    c=getchar();
    //------  restore old settings ---------
    res=tcsetattr(STDIN_FILENO, TCSANOW, &org_opts);
    assert(res==0);
    return(c);
}

int keypress(unsigned char echo)
{
    struct termios savedState, newState;
    int c;

    if (-1 == tcgetattr(STDIN_FILENO, &savedState))
    {
        return EOF;     /* error on tcgetattr */
    }

    newState = savedState;

    if ((echo = !echo)) /* yes i'm doing an assignment in an if clause */
    {
        echo = ECHO;    /* echo bit to disable echo */
    }

    /* disable canonical input and disable echo.  set minimal input to 1. */
    newState.c_lflag &= ~(echo | ICANON);
    newState.c_cc[VMIN] = 1;

    if (-1 == tcsetattr(STDIN_FILENO, TCSANOW, &newState))
    {
        return EOF;     /* error on tcsetattr */
    }

    c = getchar();      /* block (withot spinning) until we get a keypress */

    /* restore the saved state */
    if (-1 == tcsetattr(STDIN_FILENO, TCSANOW, &savedState))
    {
        return EOF;     /* error on tcsetattr */
    }

    return c;
}

int main(int argc, char **argv){
/*
    int sockfd, newsockfd, portno;
    socklen_t clilen;
    char buffer[256];
    struct sockaddr_in serv_addr, cli_addr;
    int n;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = 1790;//atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    if (bind(sockfd, (struct sockaddr *) &serv_addr,
             sizeof(serv_addr)) < 0)
        error("ERROR on binding");
    listen(sockfd,5);
    clilen = sizeof(cli_addr);
    newsockfd = accept(sockfd,
                       (struct sockaddr *) &cli_addr,
                       &clilen);
    if (newsockfd < 0)
        error("ERROR on accept");
    bzero(buffer,256);

    int converted = htonl(4);

    n = write(newsockfd,"a",2);
    if (n < 0)
    {
        error("ERROR writing to socket");
        close(newsockfd);
        close(sockfd);
    }

    int znak;
    printf("press a key to continue\n");


    znak = keypress(0);

    while (znak != 'q'){
        if(znak == 'a') {
            n = write(newsockfd, "a", 2);
            printf("poslal som a\n");
        }
        if(znak == 'w') {
            n = write(newsockfd, "w", 2);
            printf("poslal som w\n");
        }
        if(znak == 'd') {
            n = write(newsockfd, "d", 2);
            printf("poslal som d\n");
        }
        if(znak == 's') {
            n = write(newsockfd, "s", 2);
            printf("poslal som s\n");
        }
        if (n < 0)
        {
            error("ERROR writing to socket");
            close(newsockfd);
            close(sockfd);
        }

        znak = keypress(0);
    }

    close(newsockfd);
    close(sockfd);
*/

    char server_reply[10000];

    int header_received = 0, packet_position=0, packets=0;
    int sensor_socket;
    sensor_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (sensor_socket < 0)
        error("ERROR opening socket");

    struct sockaddr_in sensor_serv_addr;
    sensor_serv_addr.sin_port = htons(atoi("3000"));
    sensor_serv_addr.sin_family = AF_INET;
    sensor_serv_addr.sin_addr.s_addr = inet_addr("192.168.123.245");

    if (connect(sensor_socket,(struct sockaddr *)&sensor_serv_addr,sizeof(sensor_serv_addr)) < 0){
        printf("\nError: Connect failed\n");
        return -1;
    }

    while (1) {
        int pocet = recv(sensor_socket, server_reply, 2048, 0);

        if (pocet == 0 || pocet < 0) {

            printf("server reply: ");

        }

        else
        {
            for (int n = 0; n < pocet - 5; n++) {

                if(server_reply[n] == 0 &&
                   server_reply[n+1] == 8 &&
                   server_reply[n+2] == -36 &&
                   server_reply[n+3] == 0 &&
                   server_reply[n+4] == 0 &&
                   server_reply[n+5] == 0) //&&
                    //server_reply[n+6] == 0 &&
                    //server_reply[n+7] == 0 &&
                    //server_reply[n+8] == 3 &&
                    //server_reply[n+9] == 1)
                {

                    // header_received=1;
                    memcpy(buff_packet,&server_reply[n],sizeof(server_reply));
                    // packet_position = packet_position + pocet;
                    parse_data();

                    if(buff_packet[68]==127 && buff_packet[69]==31) {
                        printf("Vpravo nic\n");
                        buff_packet[0]='\0';
                       // break;
                    } else{
                        printf("Vpravo nieco\n");
                        buff_packet[0]='\0';
                        //break;
                    }

                    //if(buff_packet[1513]!=127 && buff_packet[1514]!=31)
                    //  printf("Vlavo nic\n");
                }
            }
        }

            /*
        else {

            if(header_received == 0)
            {
                for (int n = 0; n < pocet - 9; n++) {

                    if(server_reply[n] == 0 &&
                      server_reply[n+1] == 8 &&
                      server_reply[n+2] == -36 &&
                      server_reply[n+3] == 0 &&
                      server_reply[n+4] == 0 &&
                      server_reply[n+5] == 0) //&&
                      //server_reply[n+6] == 0 &&
                      //server_reply[n+7] == 0 &&
                      //server_reply[n+8] == 3 &&
                      //server_reply[n+9] == 1)
                      {

                       // header_received=1;
                        memcpy(buff_packet,&server_reply[n],sizeof(server_reply));
                       // packet_position = packet_position + pocet;

                        if(buff_packet[68]==127 && buff_packet[69]==31) {
                            printf("Vpravo nic\n");
                            buff_packet[0]='\0';
                            break;
                        } else{
                            printf("Vpravo nieco\n");
                            buff_packet[0]='\0';
                            break;
                        }

                        //if(buff_packet[1513]!=127 && buff_packet[1514]!=31)
                          //  printf("Vlavo nic\n");
                    }
                }
            }

            else{

                memcpy(buff_packet + packet_position, server_reply, pocet);
                packets++;
                packet_position = packet_position+pocet;
                parse_data();
                packet_position = 0;
                header_received = 0;
            //    buff_packet[0]='/0';
                //break;//*******************************
            }


            //printf("server reply: ");

        }*/
      // packet_position = 0;
     //   delete []buff_packet;
/*
            if(header_received==0){

                for (int i = 0; i < pocet-9; i++) {

                    if(server_reply[i] == 0 &&
                       server_reply[i+1] == 0 &&
                            server_reply[i+2] == 0 &&
                            server_reply[i+3] == 0 &&
                            server_reply[i+4] == 0 &&
                            server_reply[i+5] == 0 &&
                            server_reply[i+6] == 0 &&
                            server_reply[i+7] == 0 &&
                            server_reply[i+8] == 3 &&
                            server_reply[i+9] == 1){

                        header_received=1;
                        memcpy(buff_packet,server_reply,pocet);
                        packet_position = packet_position + pocet;
                    }

                }

            }
            else{

                memcpy(buff_packet + packet_position, server_reply, pocet);
                packets++;
                packet_position = packet_position+pocet;
                parse_data();
                packet_position = 0;
                header_received = 0;
            }

    }
*/
    }

  //  for (int g = 0; g < 1000; ++g) {
    //    printf("%f/n",vektorDistance[g]);
    //}

 return 0;
}

void parse_data(){
    for (int i = 0; i < 289; ++i) {

       // char ccc = (char)-36;
       // int cislo1 = (int)buff_packet[68+i*5];
       // int cislo2 = (int)buff_packet[69+i*5];

        double x = buff_packet[66+i*5] + buff_packet[67+i*5] + 128;
        double z = buff_packet[68+i+5] + buff_packet[69+i*5] + 128;
        double z1 = buff_packet[68+i*5];
        double z2 = buff_packet[69+i*5];
        //double z1 = 60 + 80 * ((double)cislo2/4095);

        std::bitset<7> bin_z1((int)z1);
      //  std::cout << bin_z1;
        std::bitset<7> bin_z2((int)z2);
        std::cout << bin_z1.to_string();

        std::string retazec1 = bin_z2.to_string() + bin_z1.to_string();
//                toBinary((int)bin_z1.to_ulong()) + toBinary((int)bin_z2.to_ulong());
        std::bitset<14> baz(retazec1);
        //baz.to_ulong();
       // long vysledok = toDecimal(retazec1);
        vektorDistance[poradie++] = baz.to_ulong();

        //if(z != 60)
          //  printf("vypis");

        printf("/f/n",z);
    }
    printf("ok/n");
}

std::string toBinary(int n)
{
    //int n = stoi
    std::string r;
    while(n!=0) {r=(n%2==0 ?"0":"1")+r; n/=2;}
    return r;
}

long toDecimal(std::string binary1)
{

    std::bitset<8> baz (std::string(binary1));

    /*
    long num = atoi(binary1.c_str());
    long bin, dec = 0, rem, base = 1;

    while (num > 0)
    {
        rem = num % 10;
        dec = dec + rem * base;
        base = base * 2;
        num = num / 10;
    }*/

    return 100;
}