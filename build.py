ino = open("arduino/main.ino", "r")
ino_text = ino.read()
ino.close()

html = open("web/index.html", "r")
html_text = html.read()
html.close()

css = open("web/style.css", "r")
css_text = css.read()
css.close()

js = open("web/index.js", "r")
js_text = js.read()
js.close()

start_position = html_text.find("<link")
end_position = html_text.find("</head>")

html_text = html_text[:start_position] + "<style>" + css_text + "</style>" + html_text[end_position:]

start_position = html_text.find("<script") + 7
end_position = html_text.find("</script>")

html_text = html_text[:start_position] + "> " + js_text + html_text[end_position:]

start_position = ino_text.find("=====(") + 6
end_position = ino_text.find(")=====")

ino_text = ino_text[:start_position] + html_text + ino_text[end_position:]

result = open("monkey.ino", "w")
result.write(ino_text)
result.close()
