/*
 * indicator.vala
 * 
 * Copyright 2014 rgoomes <rgoomes@tetrapack>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 */

using AppIndicator;

public class Request {
	// → SOURCE = APP/DOC/FOLDER/IMAGE ETC..
	//
	// REQUEST WILL HAVE:
	// → ICON PATH OF THE SOURCE (EMPRY MEANS NO ICON)
	// → PATH OR COMMAND TO START/OPEN THE SOURCE (EMPTY MEANS NO EXEC LIKE REALTIME MATH CALCULATIONS)
	// → TYPE (USEFULL TO USE GROUPS)
	// → LABEL OF THE SOURCE
	// → ...
}

class UpdaterThread {
	Appindicator app;

	public UpdaterThread(Appindicator app){
		this.app = app;
	}

	public void* thread_func(){
		int i = 0;

		while(true) {
			stdout.printf("%d\n", ++i);

			var item = new Gtk.MenuItem.with_label(i.to_string());
			item.activate.connect(() => {
				stdout.printf("please\n");
			});
			item.show();
			this.app.getMenu().append(item);
			this.app.getIndicator().set_menu(this.app.getMenu());

			Thread.usleep(1000000);
		}
	}
}

public class SearchItem:Gtk.MenuItem{
	//Gtk.Entry entry;
	Gtk.Label name;

	public SearchItem(){
		name = new Gtk.Label("Search:");
		
		/* NOT WORKING - WIDGET NOT SUPORTED
		entry = new Gtk.Entry();
		entry.set_text("asdasd");
		entry.show();
		add(entry);
		*/
		
		add(name);
		show();
	}
}

public class Menu:Gtk.Menu {
	SearchItem search_item;

	public Menu() {
		search_item = new SearchItem();

		Gtk.SeparatorMenuItem separator = new Gtk.SeparatorMenuItem();
		separator.show();

		append(search_item);
		append(separator);
	}
}

class Appindicator {
	Indicator indicator;
	Menu menu;

	public Appindicator(string name, string icon_path) {
		indicator = new Indicator(name, icon_path, IndicatorCategory.APPLICATION_STATUS);
		indicator.set_status(IndicatorStatus.ACTIVE);

		menu = new Menu();
		indicator.set_menu(menu);

		// OTHER INITS
		this.init_threads();
		Gtk.main();
	}

	public void init_threads(){
		var updater = new UpdaterThread(this);
		unowned Thread<void*> thread = Thread.create<void*>(updater.thread_func, true);
	}

	public Gtk.Menu getMenu(){
		return this.menu;
	}

	public Indicator getIndicator(){
		return this.indicator;
	}
}
