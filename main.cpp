#include <bits/stdc++.h>
#define NIVEL_MAXIM  10000

using namespace std;

///penru valoare random
float frand()
{
    return (float) rand() / RAND_MAX;
}

///pentru nivel random
int random_nivel()
{float P = 0.5;
static bool first = true;

    if (first)
    {
        srand((unsigned)time(NULL));
        first = false;
    }
    int nivel_nou = (int)(log(frand()) / log(1.-P));


if(nivel_nou< NIVEL_MAXIM)
    return nivel_nou;

else
   return NIVEL_MAXIM;

}


struct nod
{
    int valoare;
    nod **adrese; ///adresele pe niveluri
    nod(int nivel, int &valoare)  ///adaugare nod pe nivel
    {
        adrese = new nod * [nivel + 1];
        memset(adrese, 0, sizeof(nod*) * (nivel + 1)); ///initiaizam cu 0
        this->valoare = valoare;
    }
    ~nod() ///destructor
    {
        delete [] adrese;
    }
};

struct skiplist
{
    nod *header; ///header e inceputul listei
    int valoare;
    int nivel;
    skiplist() ///constructor
    {
        header = new nod(NIVEL_MAXIM, valoare);
        nivel = 0;
    }
    ~skiplist() ///destructor
    {
        delete header;
    }

void inserare(int &valoare)
{
    nod *x = header;
    nod *update[NIVEL_MAXIM + 1];

    memset(update, 0,  sizeof(nod*) * (NIVEL_MAXIM + 1)); ///initializam update ul cu 0

    for (int i = nivel;i >= 0;i--) ///verificam fiecare nivel de sus in jos si cautam cea mai mare val
                                            ///mai mica decat valoarea de inserat
    {
        while (x->adrese[i] != NULL && x->adrese[i]->valoare < valoare)
        {
            x = x->adrese[i];
        }
        update[i] = x;
    }
    x = x->adrese[0]; ///trecem la nivelul 0

    if (x == NULL || x->valoare != valoare) ///daca mai exista un nod si acesta nu are valoarea de inserat
    {
        int nivel_nou = random_nivel();

        if (nivel_nou > nivel)  ///daca nivelul generat este mai mare atunci pt fiecare nivel nou
                                          /// in update punem adresa de inceput
        {
            for (int i = nivel + 1;i <= nivel_nou;i++)
            {
                update[i] = header;
            }
            nivel = nivel_nou;  ///nivel ia valoarea noua
        }
        x = new nod(nivel_nou, valoare); ///adaugam nodul pe nivelul ales random

        for (int i = 0;i <= nivel_nou;i++) ///apoi facem legatura cu celelalte noduri intre care este inserat
        {
            x->adrese[i] = update[i]->adrese[i];
            update[i]->adrese[i] = x;
        }
    }

}


void stergere(int &valoare)
{
    nod *x = header;
    nod *update[NIVEL_MAXIM + 1];
    memset (update, 0, sizeof(nod*) * (NIVEL_MAXIM + 1));

    for (int i = nivel;i >= 0;i--)  ///cautam cea mai  mica valoare mai mare ca valoarea ce trebuie stearsa
    {
        while (x->adrese[i] != NULL && x->adrese[i]->valoare < valoare)
        {
            x = x->adrese[i];
        }
        update[i] = x;
    }
    x = x->adrese[0]; ///urmatorul nod de pe nivelul 0

    if (x->valoare == valoare)
    {
        for (int i = 0;i <= nivel;i++)
        {
            if (update[i]->adrese[i] != x) ///daca nu e pe nivelul curent atunci nu va fi nici
                                              ///pe cele de mai sus deci oprim cautarea
                  break;

            update[i]->adrese[i] = x->adrese[i]; ///facem legaturile intre vecinii nodului sters
        }
        delete x; ///il stergem

        while (nivel > 0 && header->adrese[nivel] == NULL)  ///stergem nivelurile care au ramas goale
        {
            nivel--;
        }
    }
}


void afisare()
{
    const nod *x = header->adrese[0]; ///adresa primului nod

    while (x != NULL) ///cat timp mai avem noduri le afisam valoarea
    {
        cout << x->valoare<<" ";
        x = x->adrese[0];

    }
    cout <<endl;
}

void succesor(int &valoare)
{
    const nod *x = header;
        for (int i = nivel;i >= 0;i--) ///incepem cautarea de la nivelul cel mai mare
    {
        while (x->adrese[i] != NULL && x->adrese[i]->valoare < valoare ) ///cautam pana cand valoarea urmatorului nod e mai mica
        x = x->adrese[i];


    }

   x = x->adrese[0];///trecem pe nivelul 0

      if( x->adrese[0]!=NULL  && x->valoare==valoare)
        cout<<"Succesorul lui este: "<<x->adrese[0]->valoare;
    else
        cout<<"Nu are succesor";


    cout <<endl;
}

void predecesor(int &valoare)
{
    const nod *x = header;
       if(x->adrese[0]->valoare==valoare) ///daca e primul nod pe nivelul 0 aunci nu are predecesor
        cout<<"Nu are predecesor";
    else
   {

      for (int i = nivel;i >= 0;i--)
    {
        while (x->adrese[i] != NULL && x->adrese[i]->valoare < valoare ) ///cautam pana cand valoarea urmatorului nod e mai mica
        x = x->adrese[i];


    }

      if( x->adrese[0]!=NULL  && x->adrese[0]->valoare==valoare) /// ne aflam pe cel mai mare nod mai mic decat valoarea data
        cout<<"Predecesorul  lui este: "<<x->valoare; ///daca urmatorul este nodul cautat atunci afisam nodul curent

    else cout<<"Nu are predecesor";
 }

    cout <<endl;
}




bool contine(int &valoare)
{
    nod *x = header; ///primeste adresa inceputului listei

    for (int i = nivel;i >= 0;i--)  ///incepem de la ultimul nivel si tot coboram
    {
        while (x->adrese[i] != NULL && x->adrese[i]->valoare < valoare) ///cautam prin noduri cat timp valoarea nodului
                                                                       /// curent este mai mica decat valoarea cautata
        {
            x = x->adrese[i];
        }
    }
    x = x->adrese[0]; ///adresa urmatorului nod de pe nivelul 0

    return x != NULL && x->valoare == valoare; ///daca adresa nu este nula si valoarea nodului este egala
                                              ///cu valoarea cautata atunci returneaza 1 altfel 0
}

};

int main()
{
    skiplist Lista;
    int comanda, n,k=0;
    while (k==0)
    {

        cout<<"1.Inserare"<<endl;
        cout<<"2.Stergere"<<endl;
        cout<<"3.Cautare"<<endl;
        cout<<"4.Afisare  "<<endl;
        cout<<"5.Predecesor "<<endl;
        cout<<"6.Succesor "<<endl;
        cout<<"7.Oprire "<<endl;

        cin>>comanda;
        switch(comanda)
        {

        case 1:
             cout<<"Valoarea de inserat: ";
             cin>>n;
             Lista.inserare(n);

                 cout<<endl;
             break;
        case 2:

             cout<<"Valoarea de sters: ";
             cin>>n;
             if(!Lista.contine(n))
             {
                 cout<<"Valoarea nu este in lista"<<endl;
                 break;
             }
             Lista.stergere(n);

                cout<<endl;
             break;
        case 3:

             cout<<"Valoarea de cautat: ";
             cin>>n;
             if(Lista.contine(n))
                 cout<<"DA, valoarea "<<n<<" este in lista!"<<endl;
             else
                 cout<<"NU, valoarea "<<n<<" nu este in lista!"<<endl;
                cout<<endl;
        case 4:
             cout<<"Lista : ";
             Lista.afisare();
             break;



        case 5:
          cout<<"Predecesorul lui: ";
          cin>>n;
           if(Lista.contine(n))
                    Lista.predecesor(n);
            else cout<<"Valoarea nu se afla in lista"<<endl;
           cout<<endl;
          break;



        case 6:
              cout<<"Succesorul lui :";
              cin>>n;
               if(Lista.contine(n))
                   Lista.succesor(n);
                else cout<<"Valoarea nu se afla in lista"<<endl;
             cout<<endl;
             break;


        case 7:
             k=1;
             break;

        }
    }
    return 0;
}



