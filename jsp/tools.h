# ifndef TOOLS_H_
# define TOOLS_H_

//Transforme timestamp return a string (char*)
char* TimestampToHumanString(time_t timestamp);

//tranforme timestamp ranvoie structure tm
//(tm_year/tm_mon/tm_mday)
struct tm TimestampToHumanStruct(time_t timestamp);

//osef pr l'instant
void HumanToTimestamp(void);
# endif