function doPost(e)
{
  var sheet = SpreadsheetApp.getActiveSpreadsheet().getSheetByName('Data');
  var params = e.postData.getDataAsString().split(" ");
  var co2 = (params[0]);
  var temperature = Number(params[1]);
  var humidity = Number(params[2]);
  sheet.insertRows(1,1);
  sheet.getRange(1, 1).setValue(new Date());
  sheet.getRange(1, 2).setValue(co2);
  sheet.getRange(1, 3).setValue(temperature);
  sheet.getRange(1, 4).setValue(humidity);
}
function doGet() {
  return HtmlService.createHtmlOutputFromFile('index');
}