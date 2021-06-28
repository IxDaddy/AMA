#include "offset.h"

void Offset(double *l, size_t length, int k)
{
    int len = (int)length;
    if (k > 0)
    {
        for (int i = len - 1 - k; i >= 0; i--)
        {
            l[i + k] = l[i];
        }
        for (int i = 1; i <= k; i++)
        {
            l[i] = l[0];
        }
    }
    else if (k < 0)
    {
        for (int i = k; i < len; i++)
        {
            l[i + k] = l[i];
        }
        for (int i = len - 2; i <= len + k - 1; i--)
        {
            l[i] = l[len - 1];
        }
    }
}

int HighPeriod(StockData *m, int start, int end)
{
    double high = m->close[start];
    int ihigh = start;
    for (int i = start; i < end; i++)
    {
        if (m->close[i] > high)
        {
            high = m->close[i];
            ihigh = i;
        }
    }
    return ihigh;
}

int LowPeriod(StockData *m, int start, int end)
{
    double low = m->close[start];
    int ilow = start;
    for (int i = start; i < end; i++)
    {
        if (m->close[i] < low)
        {
            low = m->close[i];
            ilow = i;
        }
    }
    return ilow;
}

void Show(int *best, size_t len, double *close)
{
    for (size_t i = 0; i < len; i++)
    {
        printf("%ld : %d,%f\n", i, best[i], close[i]);
    }
}

int *best_trade(StockData *data, int period)
{
    int range = (int)data->range;
    int *best = malloc(sizeof(int) * data->range);
    for (size_t i = 0; i < data->range; i++)
    {
        best[i] = 0;
    }
    for (int i = period; i < range; i += period)
    {
        printf("%d : %d\n", i - period, i);
        best[HighPeriod(data, i - period, i)] = -1;
        best[LowPeriod(data, i - period, i)] = 1;
    }
    //Show(best, data->range);
    return best;
}

int* ZigZag(StockData *sourceStock, double dPourcentage)
{
    int *tabZigZags = malloc(sizeof(int)*sourceStock->range);
    for (size_t i = 0; i < sourceStock->range; i++)
    {
        tabZigZags[i] = 0;
    }
    // Initialisation des variables
    // Valeur courante. J'utilise la moyenne entre plus haut, plus bas et cloture
    double dValeurMoyenneCourante = (sourceStock->close[0]) / 1;
    size_t nArraySize = sourceStock->range;

    // valeur min et max trouvée dans l'algo ainsi que les dates correspondantes..
    double dValeurMin = dValeurMoyenneCourante;
    double dValeurMax = dValeurMoyenneCourante;

    // Permet de savoir si on recherche un plus haut ou un plus bas. 
    int bPos = 0;
    int bNeg = 0;

    // Index du tableau de résultat

    // On parcourt tout les éléments du tableau
    for (size_t nIndex = 0; nIndex < nArraySize; nIndex++)
    {
        // Calcul de la valeur courante moyenne.
        dValeurMoyenneCourante = (sourceStock->close[nIndex]) / 1;

        // Test si la valeur courante est supérieure à la valeur max mémorisée
        if (dValeurMoyenneCourante > dValeurMax)
        {
            // On mémorise date et valeur max trouvées
            dValeurMax = dValeurMoyenneCourante;
            //dateMax = nIndex;

            // On recherche un plus bas, or on vient de trouver un plus haut, il faut donc réinitialiser
            // la valeur mini (c'est la partie subtile de l'algo)
            if (bNeg)
            {
                dValeurMin = dValeurMax;
            }

            // On recherche un plus bas
            if (!bNeg)
            {
                // On a trouvé un plus haut supérieur au pourcentage recherché,
                // on mémorise donc le dernier plus bas
                if ((dValeurMax - dValeurMin) / dValeurMin * 100 > dPourcentage)
                {
                    //tabZigZags[nTabIndex].date = dateMin;
                    tabZigZags[nIndex] = dValeurMin;

                    // On met à jour les paramètres
                    dValeurMin = dValeurMax;
                    bNeg = 1;
                    bPos = 0;
                }
            }
        }

        // Test si la valeur courante est inférieure à la valeur min mémorisée
        if (dValeurMoyenneCourante < dValeurMin)
        {
            // On mémorise date et valeur min trouvées
            dValeurMin = dValeurMoyenneCourante;
            //dateMin = nIndex;

            // On recherche un plus haut, or on vient de trouver un plus haut, il faut donc réinitialiser
            // la valeur maxi
            if (bPos)
            {
                dValeurMax = dValeurMin;
            }

            // On recherche un plus haut
            if (!bPos)
            {
                // On a trouvé un plus bas supérieur au pourcentage recherché,
                // on mémorise donc le dernier plus haut
                if ((dValeurMax - dValeurMin) / dValeurMax * 100 > dPourcentage)
                {
                    //tabZigZags[nTabIndex].date = dateMax;
                    tabZigZags[nIndex] = -dValeurMax;

                    // Mise à jour des paramètres
                    dValeurMax = dValeurMin;
                    bPos = 1;
                    bNeg = 0;
                }
            }
        }
    }
    return tabZigZags;  
}

int *Zigzag2(StockData *m)
{
    double percent = 0.05;
    double backstep = 5;
    double min = m->close[0];
    double max = m->close[0];
    size_t minIndex = 0;
    size_t maxIndex = 0;
    int *zigzag = malloc(sizeof(int) * m->range);
    //double Positive = 0;
    //double Negative = 0;

    for (size_t i = 0; i < m->range; i++)
    {
        zigzag[i] = 0;
    }

    for (size_t i = 0; i < m->range - backstep; i++)
    {

        if (m->close[i] > (1 + percent) * max)
        {
            max = m->close[i];
            maxIndex = i;
            size_t j = 0;

            for (; j < backstep; j++)
            {
                if (m->close[j + i] > max)
                {
                    max = m->close[i + j];
                    maxIndex = i + j;
                }
                else if (m->close[j + i] < (1 + percent) * max)
                {
                    break;
                }
            }
            if (j == backstep - 1)
            {
                zigzag[maxIndex] = -1;
            }
            i = maxIndex;
        }
        else if (m->close[i] < (1 + percent) * min)
        {
            min = m->close[i];
            minIndex = i;
            size_t j = 0;

            for (; j < backstep; j++)
            {
                if (m->close[j + i] < min)
                {
                    min = m->close[i + j];
                    minIndex = i + j;
                }
                else if (m->close[j + i] > (1 + percent) * min)
                {
                    break;
                }
            }
            if (j == backstep - 1)
            {
                zigzag[minIndex] = 1;
            }
            i = minIndex;
        }
    }
    return zigzag;
}
