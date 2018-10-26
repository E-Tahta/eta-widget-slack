// Placed in /usr/share/kde4/apps/plasma-desktop/init/
// This script is run for new users, which do not have a .kde directory
// and it set's the default wallpaper for all activities

a = activities()

for (i in a) {
    slack = a[i].addWidget("tr.org.etap.slack")
    slack.writeConfig("geometry", QRectF(1580,95,260,400))
    a[i].locked = true
}

locked = true
