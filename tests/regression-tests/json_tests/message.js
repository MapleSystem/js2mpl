var messages= [{from: "eugeneC", 
        time: "2016/2/18 14:22:10",
        content: "Hello to mapleJS on MBB!",
        read: false},
{from: "eugeneC",
        time: "2016/2/18 14:22:11",
        content: "This demos a very compact JS VM.",
        read: false},
{from: "eugeneC",
        time: "2016/2/18 14:22:12",
        content: "On a very resource constrained microcontroller.",
        read: false},
{from: "eugeneC",
        time: "2016/2/18 14:22:13",
        content: "Running pure Javascript applications.",
        read: false}
        ];
var me = JSON.stringify(messages);
//print(me);
me = JSON.parse(me);
//print(me[0].from);
if (me[0].from != "eugeneC")
  $ERROR("#1:error");
if (me[1].from != "eugeneC")
  $ERROR("#2:error");
if (me[2].from != "eugeneC")
  $ERROR("#3:error");
if (me[3].from != "eugeneC")
  $ERROR("#4:error");
if (me[0].time != "2016/2/18 14:22:10")
  $ERROR("#5:error");
if (me[1].time != "2016/2/18 14:22:11")
  $ERROR("#6:error");
if (me[2].time != "2016/2/18 14:22:12")
  $ERROR("#7:error");
if (me[3].time != "2016/2/18 14:22:13")
  $ERROR("#8:error");
if (me[0].content != "Hello to mapleJS on MBB!")
  $ERROR("#9:error");
if (me[1].content != "This demos a very compact JS VM.")
  $ERROR("#10:error");
if (me[2].content != "On a very resource constrained microcontroller.")
  $ERROR("#11:error");
if (me[3].content != "Running pure Javascript applications.")
  $ERROR("#12:error");
if(me[0].read != false || me[1].read || me[2].read || me[3].read)
  $ERROR("#13:error");

