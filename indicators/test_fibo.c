void build_chart(double val, size_t size, double* list)
{
    for (size_t i = 0; i < size; ++i)
    {
        list[i] = val;
    }
}

// graph en global

double max_stage[m->range];
double first_stage[m->range];
double second_stage[m->range];
double third_stage[m->range];
double fourth_stage[m->range];
double min_stage[m->range];




void test_fibonnacci_retracement(StockData *m)
{
    double min = m->close[0];
    double max = m->close[0];
    for (int i = 0; i < m->range; ++i)
    {
        if(min > m->close[i])
        {
            min = m->close[i];
        }
        
        if (max < m->close[i])
        {
            max = m->close[i];
        }
    }
    double difference = max - min;
    double first_lvl = max - difference*0.236;
    double second_lvl = max - difference*0.382;
    double third_lvl = max - difference*0.5;
    double fourth_lvl = max - difference*0.618;


    build_chart(max,m->range,max_stage);
    build_chart(first_lvl,m->range,first_stage);
    build_chart(second_lvl,m->range,second_stage);
    build_chart(third_lvl,m->range,third_stage);
    build_chart(fourth_lvl,m->range,fourth_stage);
    build_chart(min,m->range, min_stage);
    
}






















