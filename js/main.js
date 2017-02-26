function testAPI(input) {
	var msg = {
		TestData: document.getElementById('testdata').value
	}
	var out = 'http://127.0.0.1:8000/api/v1/foo';
	console.log(out);
	var xmlhttp = new XMLHttpRequest();
	switch(input) {
    case "POST":
        xmlhttp.open("POST", out, true);
        break;
    case "GET":
        xmlhttp.open("GET", out, true);
        break;
    // case "PUT":
    //     xmlhttp.open("PUT", out, true);
    //     break;
    // case "DELETE":
    //     xmlhttp.open("DELETE", out, true);
    //     break;
    default:
        break;
	}
	xmlhttp.send(JSON.stringify(msg));
	sendWithCheck(xmlhttp);
	loadStatus();
}

function sendWithCheck(xmlhttp) {
	xmlhttp.onload = function () {
		console.log('DONE', xmlhttp.status, xmlhttp.statusText);
		if (xmlhttp.status < 200 || xmlhttp.status > 299) {
			console.log(xmlhttp.responseText);
			window.alert("Error " + xmlhttp.status + ": " + xmlhttp.statusText + "\\n\\n" + xmlhttp.responseText);
		}
	};
	console.log('sendWithCheck', xmlhttp.responseURL);
	xmlhttp.send();
}
