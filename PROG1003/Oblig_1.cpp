/**
 * Program for å registrere, vise og slette bussruter mellom forhåndsdefinerte busstopp.
 *
 * Programmet lar brukeren opprette en rute ved å velge startstopp og deretter gyldige
 * neste stopp, basert på en tabell med kjøretid (minutter) mellom stopp.
 * 
 * @file   Oblig_1.cpp
 * @author Kristina Jonsen
 * @date   07.02.2026
 *
 */

#include <iostream>   //  Cin / Cout
#include <iomanip>    //  setw
#include <vector>     //  STLs vector
#include <string>     //  Sting-klasse
#include <cctype>     //  toupper
#include "LesData2.h" //  Lesing av div data
using namespace std;  //  std::cout / std::cin be gone


const int ANTSTOPP = 11; ///< Totalt antall ulike busstopp.


/**
 * @brief Representerer en bussrute med stopp, rutenummer og total kjøretid.
 */
struct Rute {
    vector <string> stopp; // Rutens ulike stoppesteder.
    int ruteNr,           // Reelt rutenr, f.eks. 42, 165, 718
        totMin;           // Totalt antall minutter å kjøre på ruten
};                        // (fra første til siste stoppested).

vector <Rute*> gRuter;     ///< Pekere til rutene.

const vector <string> gBusstopp = ///< Navn på alle busstopp.
{ "Skysstasjonen", "Fahlstroms plass", "Sykehuset",
  "Gjovik stadion", "Bergslia", "Overby", "Nybrua",
  "NTNU", "Kallerud", "Hunndalen", "Mustad fabrikker" };


const int gMinutter[ANTSTOPP][ANTSTOPP] = ///< Min.mellom stoppesteder.
// const vector <vector <int> > gMinutter = // Alternativt (nå lært i vår).
{ { 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // Skysstasjonen = 0
  { 3, 0, 3, 0, 0, 0, 3, 0, 0, 0, 4}, // Fahlstrøms plass = 1
  { 0, 3, 0, 1, 0, 0, 0, 0, 0, 0, 0}, // Sykehuset = 2
  { 0, 0, 1, 0, 3, 0, 0, 0, 0, 0, 0}, // Gjøvik stadion = 3
  { 0, 0, 0, 3, 0, 2, 0, 0, 0, 0, 0}, // Bergslia = 4
  { 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0}, // Øverby = 5
  { 0, 3, 0, 0, 0, 0, 0, 2, 0, 0, 2}, // Nybrua = 6
  { 0, 0, 0, 0, 0, 0, 2, 0, 0, 4, 0}, // NTNU = 7
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // Kallerud = 8
  { 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 2}, // Hunndalen = 9
  { 0, 4, 0, 0, 0, 0, 2, 0, 0, 2, 0}  // Mustad fabrikker = 10
};

void skrivMeny();
void skrivStopp();
void ruteSkrivData(const Rute rute);
void skrivRuter();
void skrivNesteStoppesteder(const int stopp);
void nyRute();
bool ruteLesData(Rute & rute);
void slettRute();
void slett(const int nr);
void slett();

/**
 * 
 * Hovedprogram.
 * 
 */
int main()  {
    char kommando;

    skrivMeny();
    kommando = toupper(lesChar("\nKommando"));

    while (kommando != 'Q') {
        switch (kommando) {
            case 'N': nyRute();     break;
            case 'S': slettRute();  break;
            case 'A': skrivRuter(); break;
            case 'B': skrivStopp(); break;
            default:  skrivMeny();  break;
        }

        kommando = lesChar("\nKommando");
    }
                                    // Fjerner alle ruter før avslutning
    slett();
    return 0;
}

/**
 * 
 * Skriver meny med tilgjengelige kommandoer.
 * 
 */
void skrivMeny() {

    cout << "\nValg: \n"
         << "\tN - Ny rute\n"
         << "\tS - slett rute\n"
         << "\tA - Skriv alle ruter\n"
         << "\tB - Skriv stopp\n"
         << "\tQ - avslutt\n";
}

/**
 * 
 * Skriver ut alle busstoppene med nummerering.
 * 
 */
void skrivStopp() {
                                    // Stoppesteder skrives ut nummerert
    cout << "\n Alle stopp:\n";
    for (int i = 0; i < gBusstopp.size(); i++) {
        cout << (i + 1) << ": " << gBusstopp[i] << '\n';
    }
}

/**
 * Skriver ut alle registrerte ruter.
 *
 * @see ruteSkrivData()
 */
void skrivRuter() {
    if (gRuter.size()> 0) {
        for (int i = 0; i < gRuter.size(); i++) {
            cout << "\nRute nr:"<< setw(2) << i+1 <<' ';
            ruteSkrivData(*gRuter[i]);
        }
    } else {
        cout << "\nIngen ruter registrert\n";
    }
}

/**
 * Skriver ut informasjon om en rute.
 *
 * @param rute - Ruten som skal skrives ut
 */
void ruteSkrivData(const Rute rute) {

    cout << "\t\nRute: " << rute.ruteNr << '\n'
         << "\tTotal minutt rute: " << rute.totMin << '\n'
         << "\tRute stopp: ";
                                    // Skriver ut stoppestedene med --> mellom
    for (int i = 0; i <rute.stopp.size(); i++){
        if (i > 0) cout << " --> ";
        cout << rute.stopp[i];
    }
        cout << "\n\n\n";
}

/**
 * Skriver ut alle lovlige neste stopp fra et gitt stopp.
 *
 * @param stopp - Indeks (0-basert) til valgt stopp i tabellen.
 */
void skrivNesteStoppesteder(const int stopp) {
    for (size_t i = 0; i < ANTSTOPP; i++) {
        if(gMinutter[stopp][i] != 0) {
            cout << i+1 << "." << gMinutter[stopp][i] << 
            " min til " << gBusstopp[i] << '\n';
        }
    }
}

/**
 * Oppretter og legger til en ny rute i oversikten.
 *
 * Leser inn en ny rute og legger den bakerst i gRuter ved vellykket innlesning.
 *
 * @see ruteLesData()
 * @see ruteSkrivData()
 */
void nyRute() {
    Rute* nyrute;
                                    // oppretter en ny rute
                                    // Nummer mellom 1 og 999 for Id på ruten.
    nyrute = new Rute;
    cout << "\nRuteNr: \n";
                                    // YAY,gikk fint aa lese inn ny rute,
                                    // legger ny rute bakerst i vectoren
    if (ruteLesData(*nyrute) == true){
        gRuter.push_back(nyrute);
        ruteSkrivData(*nyrute);
    } else {                       // Buu, gikk ikke ann å legge inn rute
                                   // Sletter ruten
        cout << "\nMisslykket innlesning";
        cout << "\nRuten slettes";
        delete nyrute;
    }
}
/**
 * Leser inn data for en rute fra bruker.
 *
 * Brukeren velger først startstopp, og deretter gyldige neste stopp (i henhold til
 * gMinutter). Innlesningen avsluttes ved å skrive 0 som neste stopp.
 *
 * @param rute  Ruten som fylles med innleste data.
 * @return true hvis ruten har minst to stopp, ellers false.
 * @see skrivStopp()
 * @see skrivNesteStoppesteder()
 */
bool ruteLesData(Rute & rute) {
    int fStopp = 0,
        nStopp = 0;
    
    rute.totMin = 0;
                                    // Leser rutenr
    rute.ruteNr = lesInt("\nRute: ", 1, 999);
                                    // Skriver ut alle stoppesteder
    skrivStopp();

    fStopp = lesInt("\nVelg Start sted: \n", 1, 11);
    rute.stopp.push_back(gBusstopp[fStopp - 1]);
                                    // Skriver ut neste lovelige stopp
    skrivNesteStoppesteder(fStopp - 1);

do {
    nStopp = lesInt("\nNeste stopp: ", 0, ANTSTOPP);

    if (nStopp == 0) {
        cout << "\n\tRuten stoppes.\n\n";
    }
    else if (gMinutter[fStopp - 1][nStopp - 1] != 0) {
        rute.stopp.push_back(gBusstopp[nStopp - 1]);
        rute.totMin += gMinutter[fStopp - 1][nStopp - 1];

        skrivNesteStoppesteder(nStopp - 1);
        fStopp = nStopp;
    }
    else {
        cout << "\nKan ikke stoppe her.\n";
    }

    } while (nStopp != 0);

    return (rute.stopp.size() > 1);

}

/**
 * Lar brukeren velge hvilken rute som skal slettes.
 *
 * Brukeren kan slette én valgt rute, eller alle ruter ved å velge -1.
 */
void slettRute() {
    int valg = 0;

    if (gRuter.size() == 0) {
        cout << "\t\nIngen ruter lagt til\n\n";
    } else {
        for (int i = 0; i < gRuter.size(); i++) {
            cout << "\n" << "Rute nummer : " << i+1; 
            ruteSkrivData(*gRuter[i]);
        }
        cout << "\n\n";

        valg = lesInt("\nHvilken rute skal slettes: (-1 for aa slette alle)", 
                        -1, gRuter.size());
                                    // Sletter alle ruter
        if(valg == -1){
            slett();
                                    // Ingen slettes, går ut av funksjonen
        }else if (valg == 0) {

            cout << "\t\nIngen rute ble fjernet, retunerer til meny\n\n";
            return;
                                    // Sletter valgt rute
        } else if(valg >= 1) {
            slett(valg);
        }
    }
}

/**
 * Sletter én rute fra oversikten og frigjør minne.
 *
 * @param nr  Nummer på ruten som skal slettes.
 */
void slett(const int nr) {
    delete gRuter[nr-1];            // Sletter valgt rute
                                    // Flytter bakeste til plassen 
                                    // sletter og peker til siste
    gRuter[nr-1] = gRuter[gRuter.size()-1]; 
    gRuter.pop_back();
    cout << "\n\tRute " << nr << " er nå slettet\n\n\n";
}

/**
 * 
 * Sletter alle ruter og tømmer oversikten.
 * 
 */
void slett() {

    for(size_t i = 0; i < gRuter.size(); i++) 
        delete gRuter[i];
        gRuter.clear();  
    
    cout << "\n\nvectoren er tom - antallet er: " << gRuter.size() << "\n\n\n";

}

