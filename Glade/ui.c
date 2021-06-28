#include <gtk/gtk.h>
#include <stdio.h>

#include "../DataDownloader.h"
#include "../plot/plot.h"
#include "../StockData.h"
#include "../strategies/strat_rsi.h"
#include "../strategies/strat_MACD.h"
#include "../strategies/strat_MA.h"
#include "../indicators/rsi.h"
#include "../indicators/macd.h"
#include "../indicators/ma.h"

#define IMAGE_LOADING "Glade/loading.png"
#define NB_INDICATORS 12

const char* list_indicators[] = {"Aucun", "Moyenne mobile", "Rsi", "Stochastique", \
    "Awesome Ocillator", "MM exponentiel", "ATR", "Bollinger", "MACD", "Ichimoku", \
    "WilliamR", "Momentum"};
const char* text_strategies[] = {"Hold", "Buy now", "Sell now"};

GtkComboBoxText *CBT_action;
GtkComboBoxText *CBT_inter;
GtkComboBoxText *CBT_range;
GtkWidget *btn_search;
GtkLabel *lbl_price;
GtkImage *image_price;
GtkImage *image_indic;
GtkGrid *grid_indicators;
GtkWidget *btn_add_indicator;
GtkLabel *label_rsi;
GtkLabel *label_macd;
GtkLabel *label_ma;

int nb_indicators = 0;

typedef struct
{
    char* action;
    char* inter;
    char* range;
} req_infos;

int init_ui()
{
    GtkBuilder      *builder; 
    GtkWidget       *window;

    gtk_init(NULL, NULL);

    builder = gtk_builder_new();
    gtk_builder_add_from_file (builder, "Glade/app.glade", NULL);

    window = GTK_WIDGET(gtk_builder_get_object(builder, "app"));
    gtk_builder_connect_signals(builder, NULL);

    btn_search = GTK_WIDGET(gtk_builder_get_object(builder, "btn_search"));
    lbl_price = GTK_LABEL(gtk_builder_get_object(builder, "lbl_price"));
    image_price = GTK_IMAGE(gtk_builder_get_object(builder, "image_price"));
    image_indic = GTK_IMAGE(gtk_builder_get_object(builder, "image_indic"));
    CBT_action = GTK_COMBO_BOX_TEXT(gtk_builder_get_object(builder, "CBT_action"));
    CBT_inter = GTK_COMBO_BOX_TEXT(gtk_builder_get_object(builder, "CBT_inter"));
    CBT_range = GTK_COMBO_BOX_TEXT(gtk_builder_get_object(builder, "CBT_range"));
    grid_indicators = GTK_GRID(gtk_builder_get_object(builder, "grid_indicators"));
    btn_add_indicator = GTK_WIDGET(gtk_builder_get_object(builder, "btn_add_indicator"));
    label_rsi = GTK_LABEL(gtk_builder_get_object(builder, "label_rsi"));
    label_macd = GTK_LABEL(gtk_builder_get_object(builder, "label_macd"));
    label_ma = GTK_LABEL(gtk_builder_get_object(builder, "label_ma"));

    g_object_unref(builder);

    gtk_widget_show(window);                
    gtk_main();

    return 0;
}

// called when window is closed
void on_window_main_destroy()
{
    remove(TEMP_GRAPH_FILENAME);
    remove(TEMP_INDIC_FILENAME);
    gtk_main_quit();
}

void update_strategies(StockData* mat)
{
    double* mm = malloc(sizeof(double) * mat->range);
    double* mm0 = malloc(sizeof(double) * mat->range);
    double* mm1 = malloc(sizeof(double) * mat->range);

    RSI(mat, mm, 20);
    double rsi = mm[mat->range - 1];

    MACD(mat, mm, mm0, mm1);
    double macd = mm[mat->range - 1];
    double macd_sig = mm0[mat->range - 1];

    MA(mat, mm, 50);
    double low_ma = mm[mat->range - 1];

    MA(mat, mm, 150);
    double high_ma = mm[mat->range - 1];

    gtk_label_set_text(label_rsi, text_strategies[StratRsiSimple(rsi, 30, 70)]);
    gtk_label_set_text(label_macd, text_strategies[StratMACDSimple(macd, macd_sig)]);
    gtk_label_set_text(label_ma, text_strategies[StartMASimple(low_ma, high_ma)]);
}

gboolean download_worker(gpointer da)
{
    req_infos* infos = da;

    int* ids = malloc(sizeof(int) * nb_indicators);
    gchar** indicators_datas = malloc(sizeof(gchar*) * nb_indicators);

    for(int i = 0; i < nb_indicators; i++)
    {
        ids[i] = gtk_combo_box_get_active(GTK_COMBO_BOX(gtk_grid_get_child_at(grid_indicators, 0, i + 1)));
        indicators_datas[i] = g_strdup(gtk_entry_get_text(GTK_ENTRY(gtk_grid_get_child_at(grid_indicators, 1, i + 1))));
        printf("%d: %s\n", ids[i], indicators_datas[i]);
    }

    StockData* data = APItoTXTAIO(infos->action, infos->inter, infos->range);
    BuildChart(data, ids, indicators_datas, nb_indicators);

    gtk_image_set_from_file(image_price, TEMP_GRAPH_FILENAME);
    gtk_image_set_from_file(image_indic, TEMP_INDIC_FILENAME);

    if(data->range > 150)
    {
        update_strategies(data);
    }

    StockData_destroy(data);

    return 0;
}

void on_btn_search_clicked()
{
    if(nb_indicators < 13)
    {
        req_infos* infos = malloc(sizeof(req_infos));
        infos->action = gtk_combo_box_text_get_active_text(CBT_action);
        infos->inter = gtk_combo_box_text_get_active_text(CBT_inter);
        infos->range = gtk_combo_box_text_get_active_text(CBT_range);

        g_idle_add(download_worker, infos);
        gtk_image_set_from_file(image_price, IMAGE_LOADING);
    }
}

void on_add_indicator()
{
    gtk_grid_insert_row(grid_indicators, 1);
    GtkWidget* new_combo_box = gtk_combo_box_text_new();
    for(size_t i = 0; i < NB_INDICATORS; i++)
        gtk_combo_box_text_append (GTK_COMBO_BOX_TEXT(new_combo_box), 0, list_indicators[i]);
    gtk_combo_box_set_active (GTK_COMBO_BOX(new_combo_box), 0);
    GtkWidget* new_entry = gtk_entry_new();
    gtk_grid_attach(grid_indicators, new_combo_box, 0, 1, 1, 1);
    gtk_grid_attach(grid_indicators, new_entry, 1, 1, 1, 1);
    gtk_widget_show_all(GTK_WIDGET(grid_indicators));
    nb_indicators++;
}