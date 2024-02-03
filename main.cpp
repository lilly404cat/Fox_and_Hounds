#include <iostream>
#include <winbgim.h>
#include <stdlib.h>
#include <conio.h>
#include <dos.h>
#include <graphics.h>
#include <mmsystem.h>

using namespace std;

int strategie=0,piesa_activa=7,matrix[8][8],rand_jucator=0,win=0,eroare_pozitie,mod_joc=1, tur = 0;
int numar=8,latime=500,inaltime=500,border_sqr,border_up,border_left,x1,y1,tipVulpe, tipCaine, u,v, game,language;;
float x,y;

void PlayervsPlayer();
void instructiune();
void deseneazaMeniul();
void deseneaza_vulpe(int j,int i);
void deseneaza_caine(int j,int i);

struct punct
{
    int i,j;
} vulpe,spre, chosen,caine_ales;

struct coordonate
{
    int y,x;
};
struct dreptunghi
{
    coordonate SS, DJ;
};

struct buton
{
    dreptunghi D;
    int culoare;
    char text[20];
};
buton B[4];   //butoanele la meniul principal
int nrButoane = 3;

int verifica_coordonate(coordonate P, dreptunghi D) //verifica-click pe butoanele din meniul principal
{
    return D.SS.x <= P.x && P.x <= D.DJ.x && D.SS.y <= P.y && P.y <= D.DJ.y;
}

void sunet()
{
    if (tur % 2 == 0)
    {
        waveOutSetVolume(0,0xFFF);
        PlaySound(TEXT("music.wav"), NULL, SND_FILENAME|SND_LOOP|SND_ASYNC);
        readimagefile("images/sunet.jpg", 850, 500, 950, 600 );
    }
    else
    {
        PlaySound(0,0,0);
        readimagefile("images/mute.jpg", 850, 500, 950, 600 );
    }
}

int butonAles() ///click pe butoanele din meniu principal
{
    int i;
    coordonate p;
    if (ismouseclick(WM_LBUTTONDOWN))
    {
        clearmouseclick(WM_LBUTTONDOWN);
        p.x = mousex();
        p.y = mousey();
        for (i = 1; i <= nrButoane; i++)
            if (verifica_coordonate(p,B[i].D))
                return i;
    }
    return 0;
}
int butonAlesJoc() //butonul-meniu din joc,nu cel din front
{
    int i, x1, y1, k;
    if (ismouseclick(WM_LBUTTONDOWN))
    {
        clearmouseclick(WM_LBUTTONDOWN);
        x1 = mousex();
        y1 = mousey();
        for (i = 1, k = 0; k < 4; i++,k++)
            if (x1 >= 835 && x1 <= 985 && y1 >= 100 + 60 * k && y1 <= 130 + 60 * k)
                return i;
    }
    return 0;
}

void loadingGame()
{
    readimagefile("images/fox.jpg", 405, 200, 495, 290);
    readimagefile("images/dog.jpg", 510, 200, 605, 290);
    rectangle(455, 395, 565, 415);
    setcolor(RED);
    for(int i = 0; i <= 100; i++)
    {
        line(460 + i, 400, 460 + i, 410);
        delay(50);
    }
}

void firstPage()
{
    readimagefile("images/front.jpg", 0, 0, 1000, 650);

    setcolor(COLOR(14, 67, 119));
    settextjustify(CENTER_TEXT,CENTER_TEXT);
    settextstyle(BOLD_FONT,HORIZ_DIR,6);
    outtextxy(500,200, (char*)"Vulpea si cainii");
    setcolor(COLOR(14, 67, 119));
    settextjustify(CENTER_TEXT,CENTER_TEXT);
    settextstyle(BOLD_FONT,HORIZ_DIR,1);
    outtextxy(500,540, (char*)"CREAT DE");
    outtextxy(500,570, (char*)"Chicu Catalina si Secrieru Maria-ELiza");

    settextstyle(8,HORIZ_DIR,1);
    outtextxy(500,20, "Apasati orice tasta pentru a continua");
    getch();
}

void deseneazaMeniul()
{
    cleardevice();
    readimagefile("images/background.jpg", 0, 0, 1000, 650);

    setcolor(COLOR(119, 0, 255)); //text meniu
    setlinestyle(0, 0, 3);
    settextjustify(CENTER_TEXT,CENTER_TEXT);
    settextstyle(BOLD_FONT,HORIZ_DIR,6);

    if (language == 1) outtextxy(500,180, (char*)"M E N I U");
    else outtextxy(500,180, (char*)"M E N U");

    settextstyle(8,HORIZ_DIR,1); //text butoane meniu
    setcolor(COLOR(0, 4, 217));
    setfillstyle(SOLID_FILL,COLOR(255, 234, 254));
    int i, k;
    for (i=1,k=0; i<=nrButoane; i++, k++)
    {
        B[i].D.SS.x=420;
        B[i].D.SS.y=240+60*k;
        B[i].D.DJ.x=580;
        B[i].D.DJ.y=270+60*k;

        switch(i)
        {
        case 1:
            if (language == 1) strcpy(B[i].text, (char*)"Cum se joaca?");
            else if(language == 2) strcpy(B[i].text, (char*)"How to play?");
            else if (language == 3) strcpy(B[i].text, (char*)"Comment jouer?");
            else strcpy(B[i].text, (char*)"Hoe te spelen?");
            break;
        case 2:
            {
            if (language == 1) strcpy(B[i].text, (char*)"Joaca");
            else if (language == 2) strcpy(B[i].text, (char*)"Play");
            else if (language == 3) strcpy(B[i].text, (char*)"Jouez");
            else strcpy(B[i].text, (char*)"Speel");
            break;
            }
        case 3:
            {
            if(language == 1) strcpy(B[i].text, (char*)"Iesire");
            else if(language == 2) strcpy(B[i].text, (char*)"Exit");
            else if (language == 3) strcpy(B[i].text, (char*)"Quittez le jeu");
            else strcpy(B[i].text, (char*)"Uitgang");
            break;
            }
        }
        rectangle(B[i].D.SS.x, B[i].D.SS.y,B[i].D.DJ.x,B[i].D.DJ.y);
        bar(B[i].D.SS.x, B[i].D.SS.y+30, B[i].D.DJ.x, B[i].D.SS.y);
        setbkcolor(COLOR(255, 234, 254));

        outtextxy(B[i].D.SS.x+80,B[i].D.SS.y+20,B[i].text);
    }
}

///functia ce permite selectarea limbii
void limba_aleasa()
{
    readimagefile("images/background.jpg", 0, 0, 1000, 650);
    setcolor(COLOR(14, 67, 119));
    settextjustify(CENTER_TEXT,CENTER_TEXT);
    settextstyle(BOLD_FONT,HORIZ_DIR,6);
    outtextxy(500,150, (char*)"Alegeti limba");

    readimagefile("images/ro.jpeg", 100, 350, 245, 450);
    readimagefile("images/en.jpg", 320, 350, 465, 450);
    readimagefile("images/fr.jpeg", 540, 350, 685, 450);
    readimagefile("images/dutch.jpg", 760, 350, 905, 450);

    int eroare = 1;
    while(eroare == 1)
    {
        if(ismouseclick(WM_LBUTTONDOWN))
        {
            clearmouseclick(WM_LBUTTONDOWN);
            u = mousex();
            v = mousey();
            eroare = 0;

            if ((u >= 100 && u <= 245) && (v >= 350 && v <= 450))
                language = 1;

            else if(u >= 320 && u <= 465 && v >= 350 && v <= 450)
                language = 2;

            else if(u >= 540 && u <= 685 && v >= 350 && v <= 450)
                language = 3;

            else if(u >= 760 && u <= 905 && v >= 350 && v <= 450)
                language = 4;
        }
    }
}

void instructiune()
{
    cleardevice();//curata ecranul
    clearviewport();
    readimagefile("images/background.jpg", 0, 0, 1000, 650);

    settextjustify(CENTER_TEXT,CENTER_TEXT); //text titlu- instructiuni
    settextstyle(BOLD_FONT,HORIZ_DIR,6);
    setlinestyle(0, 0, 2);
    setcolor(COLOR(119, 0, 255));

    if (language == 1) outtextxy(500,100,(char*)"INSTRUCTIUNI");
    else if(language == 2) outtextxy(500,100,(char*)"HOW TO PLAY");
    else if (language == 3) outtextxy(500,100,(char*)"COMMENT JOUER");
    else outtextxy(500,100,(char*)"HOE TE SPELLEN");

    setcolor(COLOR(255, 234, 254));
    rectangle(120, 160, 900, 535);

    setcolor(COLOR(255, 234, 254));
    rectangle(125, 165, 895, 530);
    setfillstyle(SOLID_FILL, COLOR(255, 234, 254));
    floodfill(126, 166, COLOR(255, 234, 254));

    setcolor(COLOR(119, 0, 255));
    settextjustify(LEFT_TEXT,RIGHT_TEXT); //text pt instructiuni
    settextstyle(8,HORIZ_DIR,2);
    switch(language)
    {
        case 1:{
            outtextxy(170,170, (char*)"Atat vulpea cat si cainii pot muta numai cate un pas ");
            outtextxy(130,200, (char*)"intr-un patratel vecin, pe diagonala si liber.");
            outtextxy(130,230, (char*)"Toate piesele se vor gasi numai pe patratelele mov,");
            outtextxy(130,260, (char*)"in orice moment al jocului. Dar in timp ce vulpea poate ");
            outtextxy(130,290, (char*)"efectua acest pas in orice directie, cainii vor muta numai ");
            outtextxy(130,320, (char*)"inainte, spre linia a 8-a a tablei, fara a se putea intoarce. ");
            outtextxy(130,350, (char*)"!!! Daca vulpea reuseste sa ajunga pe row intaia a tablaJoci");
            outtextxy(130,380, (char*)"de joc(in locul initial al cainilor),ea a scapat de urmarire,");
            outtextxy(130,410, (char*)"adica a castigat jocul.");
            outtextxy(130,440, (char*)"!!! Cand vulpea este incercuita de caini si nu mai poate face ");
            outtextxy(130,470, (char*)"nici un pas, a pierdut jocul.");
            setcolor(COLOR(119, 0, 255));
            rectangle(879, 573, 973, 595);
            settextstyle(8,HORIZ_DIR,1);
            outtextxy(882, 575, (char*)"<-INAPOI");
            break;}
        case 2:{
            outtextxy(170,170, (char*)"Both the fox and the dogs can only move one step each ");
            outtextxy(130,200, (char*)"to a neighboring square, diagonally and freely.");
            outtextxy(130,230, (char*)"All the pieces will be found only on the purple squares at any ");
            outtextxy(130,260, (char*)"moment of the game. But while the fox can perform this step in ");
            outtextxy(130,290, (char*)"any direction, the dogs will only move forward, towards the ");
            outtextxy(130,320, (char*)"8th row of the board, without being able to turn back. ");
            outtextxy(130,350, (char*)" If the fox manages to reach the first row of the game board ");
            outtextxy(130,380, (char*)"(in the original place of the dogs),it has escaped the chase, ");
            outtextxy(130,410, (char*)"which means it has won the game.");
            outtextxy(130,440, (char*)" When the fox is surrounded by dogs and cannot take another ");
            outtextxy(130,470, (char*)"step, she has lost the game.");
            setcolor(COLOR(119, 0, 255));
            rectangle(879, 573, 973, 595);
            settextstyle(8,HORIZ_DIR,1);
            outtextxy(882, 575, (char*)"<-BACK  ");
            break;
        }
        case 3:{
            outtextxy(170,170, (char*)" Le renard et les chiens ne peuvent se deplacer que ");
            outtextxy(130,200, (char*)" d'un pas chacun vers une case voisine, en diagonale et ");
            outtextxy(130,230, (char*)" librement. Toutes les pieces se retrouveront uniquement sur ");
            outtextxy(130,260, (char*)" les cases violettes a tout moment de la partie. Mais alors ");
            outtextxy(130,290, (char*)" que le renard peut effectuer ce pas dans n'importe quelle ");
            outtextxy(130,320, (char*)" direction les chiens ne feront qu'avancer, vers la 8eme ");
            outtextxy(130,350, (char*)" rangee du plateau, sans pouvoir faire demi-tour. Si le renard ");
            outtextxy(130,380, (char*)" parvient a atteindre la premiere rangee du plateau de jeu");
            outtextxy(130,410, (char*)" (a l'endroit d'origine des chiens), il a echappe a la");
            outtextxy(130,440, (char*)" poursuite, ce qui signifie qu'il a gagne la partie.");
            outtextxy(130,470, (char*)" Lorsque le renard est entoure de chiens et ne peut pas faire");
            outtextxy(130,500, (char*)" un autre pas, il a perdu la partie.");
            setcolor(COLOR(119, 0, 255));
            rectangle(879, 573, 973, 595);
            settextstyle(8,HORIZ_DIR,1);
            outtextxy(882, 575, (char*)"<-RETOUR");
            break;
        }
        case 4:{
            outtextxy(170,170, (char*)" Zowel de vos als de honden kunnen elk één stap naar");
            outtextxy(130,200, (char*)" een naburig veld zetten, diagonaal en vrij. Alle stukken");
            outtextxy(130,230, (char*)" staan op elk moment van het spel alleen op de paarse velden.");
            outtextxy(130,260, (char*)" Maar terwijl de vos deze stap in alle richtingen kan zetten,");
            outtextxy(130,290, (char*)" gaan de honden alleen vooruit, naar de 8ste rij van het bord,");
            outtextxy(130,320, (char*)" zonder terug te kunnen keren. Als de vos erin slaagt de");
            outtextxy(130,350, (char*)" eerste rij van het bord te bereiken (op de oorspronkelijke");
            outtextxy(130,380, (char*)"plaats van de honden), is hij aan de achtervolging ontsnapt,");
            outtextxy(130,410, (char*)"wat betekent dat hij het spel heeft gewonnen.");
            outtextxy(130,440, (char*)"Als de vos omringd is door honden en geen stap meer kan zetten,");
            outtextxy(130,470, (char*)"heeft ze het spel verloren.");
            setcolor(COLOR(119, 0, 255));
            rectangle(879, 573, 973, 595);
            settextstyle(8,HORIZ_DIR,1);
            outtextxy(882, 575, (char*)"<-TERUG ");
            break;
        }
    }
    int x1, y1;
    do
    {
        clearmouseclick(WM_LBUTTONDOWN);
        x1 = mousex();
        y1 = mousey();
    } while (!ismouseclick(WM_LBUTTONDOWN));

    if(y1>=573 && y1<=595 && x1>=879 && x1<=973) deseneazaMeniul();
}

void Buton_text (int x1, int y1, int x2, int y2, int borderc, int fillc, int textc, char text[], int sizetext)
{
    setcolor(borderc);
    setlinestyle(SOLID_LINE, 0xFFFF, THICK_WIDTH);
    setfillstyle(SOLID_FILL, fillc);
    rectangle(x1, y1, x2, y2);
    floodfill(int((x1+x2)/2), int((y1+y2)/2), borderc);
    setbkcolor(fillc);
    setcolor(textc);
    settextjustify (CENTER_TEXT, CENTER_TEXT);
    settextstyle (BOLD_FONT, HORIZ_DIR, sizetext);
    outtextxy(int((x1+x2)/2), int((y1+y2)/2+8), text);
}
///functia ce sterge piesa
void sterge_piese(int row, int column)
{
    int x1,y1,x2,y2;
    border_sqr=latime/numar;
    border_up=(getmaxy()-latime)/2;
    x1=border_left+border_sqr*(column);
    y1=border_up+border_sqr*(row);
    x2=x1+border_sqr;
    y2=y1+border_sqr;
    setcolor(COLOR(255, 234, 254));
    setlinestyle(SOLID_LINE,0xFFFF,1);
    setfillstyle(SOLID_FILL,COLOR(143, 113, 255));
    rectangle(x1,y1,x2,y2);
    floodfill(border_left+border_sqr*(column)+border_sqr/2,border_up+border_sqr*(row)+border_sqr/2,COLOR(255, 234, 254));
}
///functia ce deselecteaza piesa prin stergere
void notSelected(int row)
{
    int column;
    for(column=0; column<=7; column++)
        if(matrix[row][column]==0) sterge_piese(row,column);

    if(rand_jucator == 1)
    {
        if(row-2>=0)
        {
            for(column=0; column<=7; column++)
                if(matrix[row-2][column]==0)
                    sterge_piese(row-2,column);
        }
        if(row+2<=7)
        {
            for(column=0; column<=7; column++)
                if(matrix[row+2][column]==0)
                    sterge_piese(row+2,column);
        }
    }
}
///functia ce deseneaza un patrat si liniile de directie a pionului
void isSelected(int row,int column)
{
    int x1,y1,x2,y2;

    border_sqr=latime/numar;
    border_up=(getmaxy()-latime)/2;
    x1=border_left+border_sqr*(column);
    y1=border_up+border_sqr*(row);
    x2=x1+border_sqr;
    y2=y1+border_sqr;\

    setcolor(BLACK);
    setlinestyle(SOLID_LINE,0xFFFF,1);
    rectangle(x1,y1,x2,y2);
    rectangle(x1+1,y1+1,x2-1,y2-1);
    rectangle(x1+2,y1+2,x2-2,y2-2);

    if(rand_jucator==2)
    {
        if(row-1>=0 && column-1>=0)
        {
            if(matrix[row-1][column-1]==0)
                line(x1,y1,x1-border_sqr/2,y1-border_sqr/2);
        }
        if(row-1>=0 && column+1<=7)
        {
            if(matrix[row-1][column+1]==0)
                line(x1+border_sqr,y1,x1+border_sqr+border_sqr/2,y1-border_sqr/2);
        }
    }
    if(rand_jucator==1)
    {
        if(row-1>=0&&column-1>=0)
        {
            if(matrix[row-1][column-1]==0)
                line(x1,y1,x1-border_sqr/2,y1-border_sqr/2);
        }
        if(row-1>=0&&column+1<=7)
        {
            if(matrix[row-1][column+1]==0)
                line(x1+border_sqr,y1,x1+border_sqr+border_sqr/2,y1-border_sqr/2);
        }
        if(row+1<=7&&column-1>=0)
        {
            if(matrix[row+1][column-1]==0)
                line(x1,y1+border_sqr,x1-border_sqr/2,y1+border_sqr+border_sqr/2);
        }
        if(row+1<=7&&column+1<=7)
        {
            if(matrix[row+1][column+1]==0)
                line(x2,y2,x2+border_sqr/2,y2+border_sqr/2);
        }
    }
}
///functia ce deseneaza matricea jocului
void deseneaza_matrice()
{
    int i,j;
    border_sqr=latime/numar;
    border_up=(getmaxy()-latime)/2;
    border_left=(getmaxx()-inaltime)/2;
    setlinestyle(SOLID_LINE,0xFFFF,1);
    setcolor(COLOR(255, 234, 254));
    for(i=0; i<=7; i++)
        for(j=0; j<=7; j++)
        {
            rectangle(border_left+border_sqr*(i),border_up+border_sqr*(j),border_left+border_sqr*(i+1),border_up+border_sqr*(j+1));
            if((i+j)%2==0)
            {
                setfillstyle(SOLID_FILL,COLOR(255, 234, 254));
                floodfill(border_left+border_sqr*i+border_sqr/2,border_up+border_sqr*j+border_sqr/2,COLOR(255, 234, 254));
            }
            else
            {
                setfillstyle(SOLID_FILL,COLOR(143, 113, 255));
                floodfill(border_left+border_sqr*i+border_sqr/2,border_up+border_sqr*j+border_sqr/2,COLOR(255, 234, 254));
            }
        }
        Buton_text (850, 100, 970, 130, COLOR(255, 234, 254),COLOR(255, 234, 254), COLOR(143, 113, 255), "Menu",2);
        Buton_text (850, 160, 970, 190, COLOR(255, 234, 254),COLOR(255, 234, 254), COLOR(143, 113, 255), "Restart",2);
        Buton_text (850, 220, 970, 250, COLOR(255, 234, 254),COLOR(255, 234, 254), COLOR(143, 113, 255), "Exit",2);

   readimagefile("images/sunet.jpg", 850, 500, 950, 600 );
}
///functia ce misca cainele
void misca_caine(int matrix[8][8], int i1,int i2,int j1,int j2)
{

    punct shift_out_from, shift_at;
    shift_out_from.i = i1;
    shift_out_from.j = j1;
    shift_at.i = i2;
    shift_at.j = j2;
    sterge_piese(shift_out_from.i,shift_out_from.j);
    matrix[shift_at.i][shift_at.j]=matrix[shift_out_from.i][shift_out_from.j];
    matrix[shift_out_from.i][shift_out_from.j]=0;
    rand_jucator = 1;
}
///functia ce deseneaza orice piesa in joc
void deseneaza_piesa(int matrix[8][8])
{
    int row,column;
    for(row=0; row<=7; row++)
        for(column=0; column<=7; column++)
        {
            if(matrix[row][column]!=0 && matrix[row][column]!=9) deseneaza_caine(column,row);

            if(matrix[row][column]==9) deseneaza_vulpe(column,row);
        }
}
///functia ce deseneaza cainele in joc
void deseneaza_caine(int j,int i)
{
    border_sqr=latime/numar;
    border_up=(getmaxy()-latime)/2;
    border_left=(getmaxx()-inaltime)/2;
    if(tipCaine==1)
        readimagefile("images/dogIcon.jpg",border_left+border_sqr*(j),border_up+border_sqr*(i),border_left+border_sqr*(j+1),border_up+border_sqr*(i+1));
    else if(tipCaine==2)
        readimagefile("images/dogIcon1.jpg",border_left+border_sqr*(j),border_up+border_sqr*(i),border_left+border_sqr*(j+1),border_up+border_sqr*(i+1));
    else if(tipCaine==3)
        readimagefile("images/dogIcon2.jpg",border_left+border_sqr*(j),border_up+border_sqr*(i),border_left+border_sqr*(j+1),border_up+border_sqr*(i+1));

}
///functia ce deseneaza vulpea in joc
void deseneaza_vulpe(int j,int i)
{
    border_sqr=latime/numar;
    border_up=(getmaxy()-latime)/2;
    border_left=(getmaxx()-inaltime)/2;
    if(tipVulpe==1)
        readimagefile("images/foxIcon.jpg",border_left+border_sqr*(j),border_up+border_sqr*(i),border_left+border_sqr*(j+1),border_up+border_sqr*(i+1));
    else if(tipVulpe==2)
        readimagefile("images/foxIcon1.jpg",border_left+border_sqr*(j),border_up+border_sqr*(i),border_left+border_sqr*(j+1),border_up+border_sqr*(i+1));
    else if(tipVulpe==3)
        readimagefile("images/foxIcon2.jpg",border_left+border_sqr*(j),border_up+border_sqr*(i),border_left+border_sqr*(j+1),border_up+border_sqr*(i+1));

}
///functia ce returneaza daca am facut sau nu o miscare corecta
int miscare_corecta(int matrix[8][8],int i1,int i2,int j1,int j2)
{
    punct shift_at,shift_out_from;
    shift_out_from.i=i1;
    shift_out_from.j=j1;
    shift_at.i=i2;
    shift_at.j=j2;
    if( matrix[shift_at.i][shift_at.j]==0 )
    {
        if(matrix[shift_out_from.i][shift_out_from.j]==9)
        {
            if((shift_at.i == shift_out_from.i + 1 ||  shift_at.i == shift_out_from.i - 1)
                && (shift_at.j == shift_out_from.j + 1 || shift_at.j == shift_out_from.j - 1))
                return 1;
            return 0;
        }
        else
        {
            if((shift_at.i == shift_out_from.i - 1) && (shift_at.j == shift_out_from.j + 1 || shift_at.j == shift_out_from.j - 1))
                return 1;
            return 0;
        }
    }

    return 0;
}
///functia ce returneaza daca vulpea a castigat in joc
int vulpea_castigatoare(int matrix[8][8])
{
    int row,column;
    for(row=vulpe.i+1; row<=7; row++)
        for(column=0; column<=7; column++)
            if(matrix[row][column]!=0)
                return 0;
    return 1;
}
///functia ce returneaza cand cainele castiga in joc
int cainele_castigator(int matrix[8][8])
{
    int row,column;
    for(row=0; row<=7; row++)
        for(column=0; column<=7; column++)
            if((matrix[row][column]==9)&&
                    ((matrix[row-1][column-1]!=0)||(row-1)<0||(column-1)<0)&&
                    ((matrix[row-1][column+1]!=0)||(row-1)<0||(column+1)>7)&&
                    ((matrix[row+1][column-1]!=0)||(row+1)>7||(column-1)<0)&&
                    ((matrix[row+1][column+1]!=0)||(row+1)>7||(column+1)>7))
                return 1;
    return 0;
}
///functia ce creeaza matricea initiala a jocului
void creeaza_matrice(int matrix[8][8])
{
    int row,column;
    for(row=0; row<=7; row++)
        for(column=0; column<=7; column++) matrix[row][column] = 0;

    matrix[7][0] = 1;
    matrix[7][2] = 2;
    matrix[7][4] = 3;
    matrix[7][6] = 4;
    matrix[0][3] = 9;//vulpe
}
///functia ce afiseaza matricea jocului in consola
void afiseaza_matrice(int matrix[8][8])
{
    for(int i=0; i<=7; i++)
    {
        for(int j=0; j<=7; j++) cout<<matrix[i][j]<<" ";
        cout<<"\n";
    }
}

///functie ce arata cand face o miscare vulpea in joc si afiseaza textul in limba aleasa de jucator
void rand_vulpe_joc()
{
    switch(language)
            {
            case 1:
                Buton_text(20,50, 150, 100, COLOR(14, 67, 119), COLOR(14, 67, 119), COLOR(87, 222, 255), "Vulpe", 4);
                break;
            case 2:
                Buton_text(20,50, 150, 100, COLOR(14, 67, 119), COLOR(14, 67, 119), COLOR(87, 222, 255), "Fox", 4);
                break;
            case 3:
                Buton_text(20,50, 150, 100, COLOR(14, 67, 119), COLOR(14, 67, 119), COLOR(87, 222, 255), "Renard", 4);
                break;
            case 4:
                Buton_text(20,50, 150, 100, COLOR(14, 67, 119), COLOR(14, 67, 119), COLOR(87, 222, 255), "Vos ", 4);
                break;
            }
}
///functie ce arata cind vulpea face o miscare in joc
void vulpe_joc()
{
    switch (tipVulpe)
    {
    case 1:
        {
            rand_vulpe_joc();
            readimagefile("images/foxIcon.jpg", 160, 50, 210, 100);
            break;
        }
    case 2:
        {
            rand_vulpe_joc();
            readimagefile("images/foxIcon1.jpg", 160, 50, 210, 100);
            break;
        }
    case 3:
        {
            rand_vulpe_joc();
            readimagefile("images/foxIcon2.jpg", 160, 50, 210, 100);
            break;
        }
    }
}
///functie ce arata cand face o miscare cainele in joc si afiseaza textul in limba aleasa de jucator
void rand_caine_joc()
{
    switch(language)
            {
            case 1:
                Buton_text(20,50, 150, 100, COLOR(14, 67, 119), COLOR(14, 67, 119), COLOR(87, 222, 255), "Caine", 4);
                break;
            case 2:
                Buton_text(20,50, 150, 100, COLOR(14, 67, 119), COLOR(14, 67, 119), COLOR(87, 222, 255), "Dog", 4);
                break;
            case 3:
                Buton_text(20,50, 150, 100, COLOR(14, 67, 119), COLOR(14, 67, 119), COLOR(87, 222, 255), "Chien ", 4);
                break;
            case 4:
                Buton_text(20,50, 150, 100, COLOR(14, 67, 119), COLOR(14, 67, 119), COLOR(87, 222, 255), "Hond", 4);
                break;
            }
}
///functie ce arata cind cainele face o miscare in joc
void caine_joc()
{
    switch (tipCaine)
    {
        case 1:
            {
                rand_caine_joc();
                readimagefile("images/dogIcon.jpg", 160, 50, 210, 100);
                break;
            }
        case 2:
            {
                rand_caine_joc();
                readimagefile("images/dogIcon1.jpg", 160, 50, 210, 100);
                break;
            }
        case 3:
            {
                rand_caine_joc();
                readimagefile("images/dogIcon2.jpg", 160, 50, 210, 100);
                break;
            }
    }
}
///functia ce permite alegerea unui personaj
void alege_avatar()
{
    readimagefile("images/background.jpg", 0, 0, 1000, 650);
    switch(language)
    {
    case 1:
        {
            setcolor(COLOR(119, 0, 255));
            settextjustify(CENTER_TEXT,CENTER_TEXT);
            settextstyle(BOLD_FONT,HORIZ_DIR,6);
            outtextxy(500,150, (char*)"Alegeti avatarul");
            break;
        }
    case 2:
        {
            setcolor(COLOR(119, 0, 255));
            settextjustify(CENTER_TEXT,CENTER_TEXT);
            settextstyle(BOLD_FONT,HORIZ_DIR,6);
            outtextxy(500,150, (char*)"Choose your avatar");
            break;
        }
    case 3:
        {
            setcolor(COLOR(119, 0, 255));
            settextjustify(CENTER_TEXT,CENTER_TEXT);
            settextstyle(BOLD_FONT,HORIZ_DIR,6);
            outtextxy(500,150, (char*)"Choisissez votre avatar");
            break;
        }
    case 4:
        {
            setcolor(COLOR(119, 0, 255));
            settextjustify(CENTER_TEXT,CENTER_TEXT);
            settextstyle(BOLD_FONT,HORIZ_DIR,6);
            outtextxy(500,150, (char*)"Kies je avatar");
            break;
        }
    }

    readimagefile("images/foxIcon.jpg", 150, 300, 350, 500);
    readimagefile("images/foxIcon1.jpg", 400, 300, 600, 500);
    readimagefile("images/foxIcon2.jpg", 650, 300, 850, 500);

    int eroare = 1;
    while(eroare == 1)
        if(ismouseclick(WM_LBUTTONDOWN))
            {
                clearmouseclick(WM_LBUTTONDOWN);
                u = mousex();
                v = mousey();
                eroare = 0;

                if ((u >= 150 && u <= 350) && (v >= 300 && v <= 500)) tipVulpe= 1;

                else if(u >= 400 && u <= 600 && v >= 300 && v <= 500) tipVulpe= 2;

                else if(u >= 650 && u <= 850 && v >= 300 && v <= 500) tipVulpe= 3;

    readimagefile("images/dogIcon.jpg", 150, 300, 350, 500);
    readimagefile("images/dogIcon1.jpg", 400, 300, 600, 500);
    readimagefile("images/dogIcon2.jpg", 650, 300, 850, 500);

    eroare = 1;
    while(eroare == 1)
        if(ismouseclick(WM_LBUTTONDOWN))
            {
                clearmouseclick(WM_LBUTTONDOWN);
                u = mousex();
                v = mousey();
                eroare = 0;

                if ((u >= 150 && u <= 350) && (v >= 350 && v <= 500)) tipCaine= 1;

                else if(u >= 400 && u <= 600 && v >= 350 && v <= 500) tipCaine= 2;

                else if(u >= 650 && u <= 850 && v >= 350 && v <= 500) tipCaine= 3;
                            }
}
}
///functia ce afiseaza pagina de final a jocului
void End(int caz)
{
    readimagefile("images/background.jpg", 0, 0, 1000, 650);

    setcolor(COLOR(4, 243, 163));
    moveto(50,50);


    if (tipVulpe==1) readimagefile("images/foxIcon.jpg",320,190,460,330);
    else if (tipVulpe==2) readimagefile("images/foxIcon1.jpg",320,190,460,330);
    else readimagefile("images/foxIcon2.jpg",320,190,460,330);

    if (tipCaine==1) readimagefile("images/dogIcon.jpg",540,190,680,330);
    else if (tipCaine==2) readimagefile("images/dogIcon1.jpg",540,190,680,330);
    else readimagefile("images/dogIcon2.jpg",540,190,680,330);


    settextstyle(EUROPEAN_FONT, HORIZ_DIR, 6);
    settextjustify(1,1);
    if(caz==1)
    {
        setcolor(GREEN);
        setlinestyle(1, SOLID_FILL, 6);
        rectangle(320,190,460,330);
        setcolor(RED);
        setlinestyle(1, SOLID_FILL, 4);
        rectangle(540,190,680,330);

        switch(language)
        {
        case 1:
            Buton_text(145,100,895,150,COLOR(4, 243, 163), COLOR(4, 243, 163), COLOR(14, 67, 119), "Vulpea a castigat jocul!!!",5);
            break;
        case 2:
            Buton_text(150,100,850,150,COLOR(4, 243, 163), COLOR(4, 243, 163), COLOR(14, 67, 119), "The fox won the game!!!",5);
            break;
        case 3:
            Buton_text(125,100,875,150,COLOR(4, 243, 163), COLOR(4, 243, 163), COLOR(14, 67, 119), "Le renard a gagne le match!!!",5);
            break;
        case 4:
            Buton_text(75,100,925,150,COLOR(4, 243, 163), COLOR(4, 243, 163), COLOR(14, 67, 119), "De vos heeft het spel gewonnen!!!",5);
            break;

        }
    }

    else
    {
    setcolor(RED);
    setlinestyle(1, SOLID_FILL, 4);
    rectangle(320,190,460,330 );
    setcolor(GREEN);
    setlinestyle(1, SOLID_FILL, 6);
    rectangle(540,190,680,330);

    switch(language)
        {
        case 1:
            Buton_text(50,100,950,150,COLOR(4, 243, 163), COLOR(4, 243, 163), COLOR(14, 67, 119), "Cainii au castigat jocul!!!",5);
            break;
        case 2:
            Buton_text(300,100,700,150,COLOR(4, 243, 163), COLOR(4, 243, 163), COLOR(14, 67, 119), "The dogs won the game!!!",5);
            break;
        case 3:
            Buton_text(50,100,950,150,COLOR(4, 243, 163), COLOR(4, 243, 163), COLOR(14, 67, 119), "Les Chiens ont gagne le match!!!",5);
            break;
        case 4:
            Buton_text(50,100,950,150,COLOR(4, 243, 163), COLOR(4, 243, 163), COLOR(14, 67, 119), "De honden hebben het spel gewonnen!!!",5);
            break;
        }
    }
    switch(language)
        {
        case 1:
            Buton_text(300,400,700,430,COLOR(87, 222, 255), COLOR(87, 222, 255), COLOR(14, 67, 119), "Joaca din nou",3);
            break;
        case 2:
            Buton_text(300,400,700,430,COLOR(87, 222, 255), COLOR(87, 222, 255), COLOR(14, 67, 119), "Play again",3);
            break;
        case 3:
            Buton_text(300,400,700,430,COLOR(87, 222, 255), COLOR(87, 222, 255), COLOR(14, 67, 119), "Rejouer",3);
            break;
        case 4:
            Buton_text(300,400,700,430,COLOR(87, 222, 255), COLOR(87, 222, 255), COLOR(14, 67, 119), "Speel opnieuw",3);
            break;
        }
    Buton_text (300, 450, 700, 480, COLOR(87, 222, 255), COLOR(87, 222, 255), COLOR(14, 67, 119), "Menu",3);
    Buton_text(870, 590, 950, 620, COLOR(4, 243, 163), COLOR(4, 243, 163), COLOR(14, 67, 119), "Exit",3);

    mod_joc=0;
    while(mod_joc==0)
    {
        if(ismouseclick(WM_LBUTTONDOWN))
        {
            x=mousex();
            y=mousey();
            if(y>=400&&y<=430&& x >= 300 && x<= 700)
            {
                switch(language)
                {
                    case 1:
                        {
                        Buton_text(300,400,700,430,COLOR(87, 222, 255), COLOR(87, 222, 255), COLOR(14, 67, 119), "Joaca din nou",3);
                        PlayervsPlayer();
                        break;}
                    case 2:{
                        Buton_text(300,400,700,430,COLOR(87, 222, 255), COLOR(87, 222, 255), COLOR(14, 67, 119), "Play again",3);
                        PlayervsPlayer();
                        break;}
                    case 3:{
                        Buton_text(300,400,700,430,COLOR(87, 222, 255), COLOR(87, 222, 255), COLOR(14, 67, 119), "Rejouer",3);
                        PlayervsPlayer();
                        break;}
                    case 4:{
                        Buton_text(300,400,700,430,COLOR(87, 222, 255), COLOR(87, 222, 255), COLOR(14, 67, 119), "Speel opnieuw",3);
                        PlayervsPlayer();
                        break;}
        }

                mod_joc=1;
                game=3;
            }
            if(y>=450&&y<=480&& x >= 300 && x<= 700);
            {
                Buton_text (300, 450, 700, 480, COLOR(87, 222, 255), COLOR(87, 222, 255), COLOR(14, 67, 119), "Menu",3);
                mod_joc=2;
                deseneazaMeniul();
            }
            if(x >= 870 && x <= 950 && y >= 590 && y <=620)
            {
                Buton_text(870, 590, 950, 620, COLOR(4, 243, 163), COLOR(4, 243, 163), COLOR(14, 67, 119), "Exit",3);
                exit(0);
            }
        }
    }
}
///functia mouse ia coordonatele de la mouse
void mouse(int &x,int &y)
{
    if(ismouseclick(WM_LBUTTONDOWN))
    {
        clearmouseclick(WM_LBUTTONDOWN);
        x=mousex();
        y=mousey();
        if (!(x>=border_left && x<=border_left+latime && y>=border_up&&y<=border_up+inaltime))
            eroare_pozitie=1;
        ///pentru MENU si RESTART
        if(x>=850 && x<=970)
        {
            eroare_pozitie=2;
            x1=x;
            y1=y;
        }

        else
        {
            eroare_pozitie=0;
            int aux=x;
            x=(y-border_up)/border_sqr;
            y=(aux-border_left)/border_sqr;

        }
    }
}
///functia start porneste jocul mereu dupa ce un avatar face o miscare
void start(int matrix[8][8])
{
    int pozitie_gresita=0;

    if(vulpea_castigatoare(matrix)==1)
    {
        End(1);
        cout<<"Vulpea a castigat"<<endl;
    }
    else if(cainele_castigator(matrix)==1)
    {
        End(2);
        cout<<"Cainii au castigat"<<endl;
    }
    else
    {
        if(rand_jucator == 1)
        {
            vulpe_joc();
            isSelected(vulpe.i,vulpe.j);
            eroare_pozitie=1;
            while(eroare_pozitie==1) mouse(spre.i,spre.j);

            if(eroare_pozitie==0)
            {
                if(miscare_corecta(matrix,vulpe.i,spre.i,vulpe.j,spre.j))
                {
                    sterge_piese(vulpe.i,vulpe.j);
                    matrix[spre.i][spre.j]=9;
                    matrix[vulpe.i][vulpe.j]=0;
                    vulpe.i=spre.i;
                    vulpe.j=spre.j;
                    piesa_activa=vulpe.i;
                    notSelected(vulpe.i);
                    rand_jucator = 2;
                }
                else
                {
                    cout<<"Eroare"<<"\n";
                    pozitie_gresita=1;
                }
            }

            else if(eroare_pozitie==2)
            {
                if (y1>100 && y1<130)
                {
                    Buton_text (850, 100, 970, 130, COLOR(255, 234, 254),COLOR(255, 234, 254), COLOR(143, 113, 255), "Menu",3);
                    pozitie_gresita=2;
                    deseneazaMeniul();
                }
                else if (y1>160 && y1<190)
                {
                    Buton_text (850, 160, 970, 190, COLOR(255, 234, 254),COLOR(255, 234, 254), COLOR(143, 113, 255), "Restart",3);
                    pozitie_gresita=2;
                    PlayervsPlayer();
                }
                else if(y1 >= 220 && y1 <= 250)
                {
                    Buton_text (850, 220, 970, 250, COLOR(255, 234, 254),COLOR(255, 234, 254), COLOR(143, 113, 255), "Exit",2);
                    pozitie_gresita = 2;
                    exit(0);
                }
                else if(y1 >= 500 && y1 <= 600)
                {
                    tur++;
                    pozitie_gresita = 0;
                    sunet();
                }
                else
                {
                    cout<<"Eroare"<<"\n";
                    pozitie_gresita=1;
                }
            }
        }
        else
        {
            caine_joc();
            eroare_pozitie=1;
            while(eroare_pozitie==1) mouse(chosen.i,chosen.j);

            if(eroare_pozitie==0)
            {
                if(matrix[chosen.i][chosen.j]!=9 && matrix[chosen.i][chosen.j]!=0)
                {
    RANDUL_CAINILOR:
                    caine_ales.i=chosen.i;
                    caine_ales.j=chosen.j;
                    isSelected(caine_ales.i,caine_ales.j);
                    chosen.i=0;
                    chosen.j=0;
                    while(eroare_pozitie == 1 || miscare_corecta(matrix,caine_ales.i,chosen.i,caine_ales.j,chosen.j)==0)
                        {
                            mouse(chosen.i,chosen.j);
                            if(matrix[chosen.i][chosen.j]!=0&&matrix[chosen.i][chosen.j]!=9)
                            {
                                deseneaza_caine(caine_ales.j,caine_ales.i);
                                if(caine_ales.i>0)
                                notSelected(caine_ales.i-1);
                                goto RANDUL_CAINILOR;
                            }
                        }

                    misca_caine(matrix,caine_ales.i,chosen.i,caine_ales.j,chosen.j);
                    notSelected(chosen.i);
                }
                else
                {
                    cout<<"Eroare"<<"\n";
                    pozitie_gresita=1;
                }
            }
            else if(eroare_pozitie==2)
            {
                if (y1>=100 && y1 <=130)
                {
                    Buton_text (850, 100, 970, 130, COLOR(255, 234, 254),COLOR(255, 234, 254), COLOR(143, 113, 255), "Menu",2);
                    pozitie_gresita=2;
                    deseneazaMeniul();
                }
                else if (y1>= 160 && y1<= 190)
                {
                    Buton_text (850, 160, 970, 190, COLOR(255, 234, 254),COLOR(255, 234, 254), COLOR(143, 113, 255), "Restart",2);
                    pozitie_gresita=2;
                    PlayervsPlayer();
                }
                else if(y1 >= 220 && y1 <= 250)
                {
                    Buton_text (850, 220, 970, 250, COLOR(255, 234, 254),COLOR(255, 234, 254), COLOR(143, 113, 255), "Exit",2);
                    pozitie_gresita = 2;
                    exit(0);
                }
                else if(y1 >= 500 && y1 <= 600)
                {
                    tur++;
                    sunet();
                    pozitie_gresita = 0;
                }
                else
                {
                    cout<<"Eroare"<<"\n";
                    pozitie_gresita=1;
                }
            }
                   }
        if(pozitie_gresita==0)
        {
            system("cls");
            deseneaza_piesa(matrix);
            afiseaza_matrice(matrix);

        }
        if(pozitie_gresita!=2) start(matrix);
    }
}
///functia ce porneste jocul
void PlayervsPlayer()
{
    clearmouseclick(WM_LBUTTONDOWN);

    mod_joc = 1;
    vulpe.i = 0;
    vulpe.j = 3;
    rand_jucator = 1;

    clearviewport();
    sunet();
    alege_avatar();
    readimagefile("images/background.jpg", 0, 0, 1000, 650);
    creeaza_matrice(matrix);
    deseneaza_matrice();
    deseneaza_piesa(matrix);
    afiseaza_matrice(matrix);

    if(mod_joc==1) start(matrix);
}

int main()
{
    initwindow(1000, 650, (char*)"Vulpea si cainii");
    setbkcolor(LIGHTCYAN);
    loadingGame();
    firstPage();
    limba_aleasa();
    deseneazaMeniul(); //se deschide prima pagina a jocului,apasand o tasta apare meniul

    int comanda = -1, butonul_apasat;
    do
    {
        butonul_apasat=butonAles();
        switch(butonul_apasat)
        {
            case 1 : instructiune(); break;
            case 2 : PlayervsPlayer(); break;
            case 3 : return 0; break;
        }
    }
    while (comanda!=3);

    closegraph();
    return 0;
}

