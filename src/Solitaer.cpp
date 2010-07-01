/*
 * <10
 * Core2Duo 2,40 GHz G++ Cygwin -O1      0:16 
 * 
 * <11
 * Core2Duo 2,40 GHz G++ Cygwin -O1      2:38
 * 
 * <12
 * Core2Duo 2,40 GHz G++ Cygwin -O1     26:11
 * 
 * <13
 * Core2Duo 2,40 GHz G++ Cygwin -O1    4h = 4* (1*   12:39 + 2*   26:06)
 * 
 * <14
 * Core2Duo 2,40 GHz G++ Cygwin -O1   40h = 4* (1* 1:50:49 + 2* 4:09:24)
 *
 * <15
 * Core2Duo 2,40 GHz G++ Cygwin -O1  228h = 4* (2*4h + 1*8h + 2*4h + 1*6h + 1*3h + 1*12h + 1*4h +1*8h)

 * Spielfeldgroesse = 33
 * Zug   Anzahl      maximal             Anzahl                   Anzahl       Verviel-   Zerlegte Spielsimulation
 *       Spielsteine unterschiedliche    unterschiedlicher        gespielter   fachungs- 
 *                   Spielfelder         Spielfelder              Spielfelder  faktor
 *   0            32                  33                 1                  1          1
 *   1            31                 528                 1                  4          4  
 *   2            30               5.456                 2                 12          3  4* (1*              1 + 2*               1)
 *   3            29              40.920                 8                 60          5  4* (1*              5 + 2*               5) 
 *   4            28             237.336                                  400          7  4* (1*             32 + 2*              34)
 *   5            27           1.107.568                                2.960          8  4* (1*            222 + 2*             259)
 *   6            26           4.272.048                               24.600          9  4* (1*          1.670 + 2*           2.240)
 *   7            25          13.884.156                              221.072          9  4* (1*         13.938 + 2*          20.665) 
 *   8            24          38.567.100                            2.076.744         10  4* (1*        123.498 + 2*         197.844)
 *   9            23          92.561.040                           20.123.080         10  4* (1*      1.130.616 + 2*       1.950.077)
 *  10            22         193.536.720                          197.757.768         10  4* (1*     10.512.530 + 2*      19.463.456)
 *  11            21         354.817.320                        1.937.125.160         10  4* (1*     97.560.870 + 2*     193.360.210)
 *  12            20         573.166.440                       18.687.793.880         10  4* (1*    893.825.684 + 2*   1.889.061.393)       
 *  13            19         818.809.200                      175.793.675.328         10  4* (1*  8.015.846.424 + 2*  17.966.286.204)
 *  14            18       1.037.158.320                    1.594.744.777.464          9  4* (1* 69.564.839.132 + 2* 164.560.677.617)
 *  15            17       1.166.803.110                    8.051.776.580.000          5  4* (2*136.107.275.955 + 1*   5.366.157.422 + 2*149.859.845.944 + 1*241.815.615.595 + 1*138.551.634.263 + 1*506.241.345.099 + 1*172.021.355.151 + 1*377.013.793.897)
 *  16            16       1.166.803.110
 *  17            15       1.037.158.320
 *  18            14         818.809.200
 *  19            13         573.166.440
 *  20            12         354.817.320
 *  21            11         193.536.720
 *  22            10          92.561.040
 *  23             9          38.567.100
 *  24             8          13.884.156
 *  25             7           4.272.048
 *  26             6           1.107.568
 *  27             5             237.336
 *  28             4              40.920
 *  29             3               5.456
 *  30             2                 528
 *  31             1                  33
 */




#include <iostream>
#include <time.h>

const char ENDL = '\n';

const unsigned int SEITENLAENGE = 7;
const unsigned int ANZAHL_FELDER = SEITENLAENGE * SEITENLAENGE;

enum Richtung_Typ {
	OBEN=1, UNTEN=2, LINKS=4, RECHTS=8
};

unsigned short int tiefe = 0;
unsigned short int anzahlSteine = 0;

unsigned short int ZugFolge[ANZAHL_FELDER][3];

unsigned long long Spielfelder[ANZAHL_FELDER];

enum Feld_Typ {
	O, I, X
};

unsigned short int Spielfeld[SEITENLAENGE][SEITENLAENGE] = { 
		{ X, X, I, I, I, X, X }, 
		{ X, X, O, I, I, X, X }, 
		{ I, I, O, I, I, I, I }, 
		{ I, I, O, O, I, I, I },
		{ I, I, I, I, I, I, I }, 
		{ X, X, I, I, I, X, X }, 
		{ X, X, I, I, I, X, X } };

/*
+-------+
|  III  |
|  oII  |
|IIoIIII|
|IIooIII|
|IIIIIII|
|  III  |
|  III  |
+-------+

unsigned short int Spielfeld[SEITENLAENGE][SEITENLAENGE] = { 
		{ X, X, I, I, I, X, X }, 
		{ X, X, I, I, I, X, X }, 
		{ I, I, I, I, I, I, I }, 
		{ I, I, I, O, I, I, I },
		{ I, I, I, I, I, I, I }, 
		{ X, X, I, I, I, X, X }, 
		{ X, X, I, I, I, X, X } };
*/

void print_timestamp() {
	struct tm *newtime;
	time_t long_time;

	// Systemzeit als long int
	time(&long_time);
	// in struct tm umwandeln
	newtime = localtime(&long_time);
	// Sekunden [0..59]
	int nSekunden = newtime->tm_sec;
	// Minuten [0..59]
	int nMinuten = newtime->tm_min;
	// Stunden [0..23]
	int nStunden = newtime->tm_hour;
	// Tag des Monats [0..31]
	int nTag = newtime->tm_mday;
	// Monat [0..12]
	int nMonat = newtime->tm_mon;
	// Jahr (aktuelles Jahr minus 1900)
	int nJahr = newtime->tm_year;
	// Wochentag [0..6], Sonntag = 0
	int nWochentag = newtime->tm_wday;
	// Tag des Jahres [0..365], 1. Jan = 0
	int nJahresTag = newtime->tm_yday;
	// Sommerzeit? 0 = Nein  (nach amerikanischen Regeln!)
	int nDST = newtime->tm_isdst;

	std::cout << nStunden << ":" << nMinuten << ":" << nSekunden << ENDL;
	std::cout.flush();

}

void zeigeFeld(void) {
	std::cout << '+';
	for (int i = 0; i < SEITENLAENGE; i++) {
	  std::cout << '-';
	}
	std::cout << '+' << ENDL;
	for (int i = 0; i < SEITENLAENGE; i++) {
		std::cout << '|';
		for (int j = 0; j < SEITENLAENGE; j++) {
			switch (Spielfeld[i][j]) {
			case I:
				std::cout << "I";
				break;
			case O:
				std::cout << "o";
				break;
			case X:
				std::cout << " ";
				break;
			}
		}
		std::cout << '|'<< ENDL;
	}
	std::cout << '+';
	for (int i = 0; i < SEITENLAENGE; i++) {
	  std::cout << '-';
	}
	std::cout << '+' << ENDL;
}

inline void ZugEintragen(int i, int j, int richtung) {
	ZugFolge[tiefe][0] = i + 1;
	ZugFolge[tiefe][1] = j + 1;
	ZugFolge[tiefe][2] = richtung;
	tiefe++;
	anzahlSteine--;
}

inline void ZugLoeschen(void) {
	tiefe--;
	anzahlSteine++;
	ZugFolge[tiefe][0] = 0;
	ZugFolge[tiefe][1] = 0;
	ZugFolge[tiefe][2] = 0;
}

void rekursion();

void DruckeZugFolge(void) {
	std::cout << "Gewonnen,gewonnen!!!" << ENDL;
	std::cout << "Zugfolge:" << ENDL;
	int z = 0;
	while (ZugFolge[z][0] != 0) {
		std::cout << "Von Position Zeile=" << ZugFolge[z][0] << ", Spalte=" << ZugFolge[z][1]
				<< " nach ";
		switch (ZugFolge[z][2]) {
		case OBEN:
			std::cout << "oben.";
			break;
		case UNTEN:
			std::cout << "unten.";
			break;
		case LINKS:
			std::cout << "links.";
			break;
		case RECHTS:
			std::cout << "rechts.";
			break;
		}
		std::cout << ENDL;
		z++;
	}
}


inline void Weiterverzweigen(int i, int j, int richtung) { // Zug ausführen
	Spielfeld[i][j] = I;
	switch (richtung) {
	case RECHTS:
		Spielfeld[i][j - 1] = O;
		Spielfeld[i][j - 2] = O;
		ZugEintragen(i, j - 2, RECHTS);
		rekursion(); // verzweigen
		ZugLoeschen();   // Zug zurücknehmen
		Spielfeld[i][j] = O;
		Spielfeld[i][j - 1] = I;
		Spielfeld[i][j - 2] = I;
		break;
	case LINKS:
		Spielfeld[i][j + 1] = O;
		Spielfeld[i][j + 2] = O;
		ZugEintragen(i, j + 2, LINKS);
		rekursion(); // verzweigen
		ZugLoeschen();   // Zug zurücknehmen
		Spielfeld[i][j] = O;
		Spielfeld[i][j + 1] = I;
		Spielfeld[i][j + 2] = I;
		break;
	case UNTEN:
		Spielfeld[i - 1][j] = O;
		Spielfeld[i - 2][j] = O;
		ZugEintragen(i - 2, j, UNTEN);
		rekursion(); // verzweigen
		ZugLoeschen();   // Zug zurücknehmen
		Spielfeld[i][j] = O;
		Spielfeld[i - 1][j] = I;
		Spielfeld[i - 2][j] = I;
		break;
	case OBEN:
		Spielfeld[i + 1][j] = O;
		Spielfeld[i + 2][j] = O;
		ZugEintragen(i + 2, j, OBEN);
		rekursion(); // verzweigen
		ZugLoeschen();   // Zug zurücknehmen
		Spielfeld[i][j] = O;
		Spielfeld[i + 1][j] = I;
		Spielfeld[i + 2][j] = I;
		break;
	}
}

void rekursion() {
	Spielfelder[tiefe]++;

	if (tiefe==12) return;

	if (tiefe==1) {
		print_timestamp();
		std::cout << Spielfelder[tiefe] << ENDL;
	}

//	if ((anzahlSteine == 1) && (Spielfeld[3][3] == I)) {
//		DruckeZugFolge();
//	} else {
		for (int i = 0; i < SEITENLAENGE; i++) {
			for (int j = 0; j < SEITENLAENGE; j++) {
				if (Spielfeld[i][j] == O) {
					if ((j - 2 >= 0) 
							&& (Spielfeld[i][j - 2] == I)
							&& (Spielfeld[i][j - 1] == I))
						Weiterverzweigen(i, j, RECHTS);
					if ((j + 2 < SEITENLAENGE) 
							&& (Spielfeld[i][j + 2] == I)
							&& (Spielfeld[i][j + 1] == I))
						Weiterverzweigen(i, j, LINKS);
					if ((i - 2 >= 0) 
							&& (Spielfeld[i - 2][j] == I)
							&& (Spielfeld[i - 1][j] == I))
						Weiterverzweigen(i, j, UNTEN);
					if ((i + 2 < SEITENLAENGE) 
							&& (Spielfeld[i + 2][j] == I)
							&& (Spielfeld[i + 1][j] == I))
						Weiterverzweigen(i, j, OBEN);
				}
			}
		}
//	}
}

int main() {

	std::cout << "max int = " << ((int)(1024*1024*1024*2-1)) << ENDL;

	for (int i = 0; i < ANZAHL_FELDER; i++) {
		Spielfelder[i] = 0;
	}
	std::cout << "Spielfeld zu Beginn" << ENDL;
	zeigeFeld();
	for (int i = 0; i < SEITENLAENGE; i++)
		for (int j = 0; j < SEITENLAENGE; j++)
			if (Spielfeld[i][j]==I) anzahlSteine++;
	std::cout << "Anzahl Steine: " << anzahlSteine << ENDL;

	std::cout << "Starte Spielsimulation um ";
	print_timestamp();
	rekursion();
	std::cout << "Ende erreicht um ";
	print_timestamp();

	std::cout << "Statistik" << ENDL;
	std::cout << "Zug\tAnzahl Spielfelder" << ENDL;
	for (int i = 0; i < ANZAHL_FELDER && Spielfelder[i]>0; i++) {
		std::cout << i << "\t" << Spielfelder[i] << ENDL;
	}
	return 0;
}
