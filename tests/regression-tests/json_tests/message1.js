    var    messages = [{from: "eugeneC", 
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

    var    num_read_messages = 0;
//}

var state = JSON.stringify({messages: messages, read_count: num_read_messages});
var x = JSON.parse(state);
if (x.read_count !== 0)
  $ERROR("#1 error");
if (x.messages[0].from != "eugeneC")
  $ERROR("#2 error");
if (x.messages[3].content != "Running pure Javascript applications.")
  $ERROR("#3 error");


