/*
Steam Table GUI Implementation
using freesteam IAPWS-IF97 steam tables library and Gtk+2 toolkit
Copyright (C) 2010  Carlo Tegano

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

#include "freesteam-gtk.h"

int
main (int argc, char *argv[])
{
    GtkBuilder      *builder; 
    GtkWidget       *window;
    GtkTreeIter iter;

    gtk_init (&argc, &argv);
 
    builder = gtk_builder_new ();
    gtk_builder_add_from_file (builder, UI_FILE, NULL);
    window = GTK_WIDGET (gtk_builder_get_object (builder, "window"));

    /* define and fill in the data structure containing steam table data */
    TableData data;
    
    data.iter = &iter;
    data.solver_status = GTK_STATUSBAR( gtk_builder_get_object (builder, "statusbar"));

    data.indip_var1_entry = GTK_ENTRY( gtk_builder_get_object (builder, "indip_var1_entry"));
    data.indip_var2_entry = GTK_ENTRY( gtk_builder_get_object (builder, "indip_var2_entry"));

    data.SolverClassCombo  = GTK_COMBO_BOX( gtk_builder_get_object (builder, "solver_class_combo") );
    data.IndVariablesCombo = GTK_COMBO_BOX( gtk_builder_get_object (builder, "indip_variables_combo") );

    data.list                    = GTK_LIST_STORE( gtk_builder_get_object (builder, "liststore"));
    data.IndVarsSinglePhaseModel = GTK_LIST_STORE( gtk_builder_get_object (builder, "indip_vars_single_phase_liststore"));
    data.IndVarsSaturationModel  = GTK_LIST_STORE( gtk_builder_get_object (builder, "indip_vars_saturation_liststore"));

    data.IndVar1Label = GTK_LABEL( gtk_builder_get_object (builder, "indip_var1_label") );
    data.IndVar2Label = GTK_LABEL( gtk_builder_get_object (builder, "indip_var2_label") );

    data.about = GTK_DIALOG (gtk_builder_get_object (builder, "aboutdialog"));

    gtk_builder_connect_signals (builder, &data);

    gtk_statusbar_push( data.solver_status, 0, "Ready");

    /*
    g_object_unref (G_OBJECT (builder));
    */

    gtk_widget_show (window);             
  
    gtk_main ();

    return 0;
}


