#define PIN_PAGE_LEN 1580
#define PIN_PAGE "\
<html>\
<head>\
<meta content='text/html; charset=ISO-8859-1' http-equiv='content-type'>\
<title>Pin Setup</title>\
<style> body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }</style>\
</head>\
<body>\
<h1>Pin Setup</h1>\
<form action='/pin' method='post'>\
<table style='width: 516px; height: 263px;' summary='' border='1' cellpadding='1' cellspacing='1'>\
<tbody>\
<tr>\
<th><input value='Save' formmethod='post' formaction='/Ppost' formtarget='_self' type='submit'></th>\
<th>UP</th>\
<th>DOWN</th>\
</tr>\
<tr>\
<td colspan='3'><b>Window 1</b></td>\
</tr>\
<tr>\
<td>Switches</td>\
<td><input size='2' maxlength='2' value='%00d' name='W1su' type='number'> 0,2,5,4</td>\
<td><input size='2' maxlength='2' value='%00d' name='W1sd' type='number'> 2,0,5,4</td>\
</tr>\
<tr>\
<td style='width: 100px; height: 31px;'>Relays</td>\
<td><input size='2' maxlength='2' value='%00d' name='W1ru' type='number'> 12,13,14,16</td>\
<td><input size='2' maxlength='2' value='%00d' name='W1rd' type='number'> 13,12,14,16</td>\
</tr>\
<tr>\
<td colspan='3'><b>Window 2</b></td>\
</tr>\
<tr>\
<td>Switches</td>\
<td><input size='2' maxlength='2' value='%00d' name='W2su' type='number'> 5,4,0,2</td>\
<td><input size='2' maxlength='2' value='%00d' name='W2sd' type='number'> 4,5,0,2</td>\
</tr>\
<tr>\
<td>Relays</td>\
<td><input size='2' maxlength='2' value='%00d' name='W2ru' type='number'> 14,16,12,13</td>\
<td><input size='2' maxlength='2' value='%00d' name='W2rd' type='number'> 16,14,12,13</td>\
</tr>\
</tbody>\
</table>\
</form>\
</body>\
</html>"
