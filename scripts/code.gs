function doPost(e)
{
  var sheet = SpreadsheetApp.getActiveSpreadsheet().getSheetByName('Data');
  var params = e.postData.getDataAsString().split(" ");
  sheet.insertRows(1,1);
  sheet.getRange(1, 1).setValue(new Date());
  for(i=0;i<params.length;i++){
    sheet.getRange(1, 2+i).setValue(Number(params[i]));
  }
}
function doGet(e) {
  var html = HtmlService.createTemplateFromFile('index');
  return html.evaluate();
}

function getSheetUrl(dateString) {
  //Date
  var date = new Date(new Date().getFullYear(), new Date().getMonth(), 0, 0, 0, 0);
  var q =  dateString.replace(/[^0-9]/g, "");
  if(q.length == 8)date = new Date(Number(q.slice(0,4)), Number(q.slice(4,6))-1, Number(q.slice(6,8)), 0, 0, 0,);
  var dateNext = new Date(date);
  dateNext.setDate(dateNext.getDate()+1);
  //Range
  var sheet = SpreadsheetApp.getActiveSpreadsheet();
  var values = sheet.getRange("A:A").getValues();
  var index = 0;
  var indexNext = 0;
  var i=0;
  while(new Date(values[i]) > dateNext && i<values.length)i++;
  indexNext = i+1;
  while(new Date(values[i]) > date && i<values.length)i++;
  index = i+1;
  //URL
  var ssid = SpreadsheetApp.getActiveSpreadsheet().getId();
  return "https://docs.google.com/spreadsheets/d/"+ssid+"/gviz/tq?range=A" +indexNext +":E"+index;
}