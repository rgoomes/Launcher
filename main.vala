/*
 * main.vala
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

 /*
	COMPILE WITH:
	-> valac --pkg appindicator3-0.1 --pkg gtk+-3.0 --thread --target-glib 2.36 --disable-warnings main.vala indicator.vala
 */

class Main {

	public static int main(string[] args) {
		Gtk.init(ref args);

		if(!Thread.supported()){
			stdout.printf ("No thread support. Exiting..\n");
			return -1;
		}

		Appindicator indapp = new Appindicator("Launcher", "/home/rgoomes/Desktop/Util/Launcher/teste.svg"); // FULL PATH

		return 0;
	}
}