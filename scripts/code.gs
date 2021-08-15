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
function doGet() {
  var html = HtmlService.createTemplateFromFile('index');
  html.url = SpreadsheetApp.getActiveSpreadsheet().getUrl();
  return html.evaluate();
}