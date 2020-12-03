# Open Graph Drawing Framework

The code is customized by me. See **Modification** section.

You have to compile the OGDF manually using Static Runtime (`/MT` / `/MTd`) or download the precompiled library [here](https://drive.google.com/drive/folders/1E7XeW_xVMWMtdrwwZib1tQOtaxiUJv3A?usp=sharing)

## Modification

Here is what I have done to compile the library

Modify `OGDF-snapshot\src\ogdf\fileformats\SvgPrinter.cpp`, look for `if (m_attr.has(GraphAttributes::nodeLabel)) {` line and change it into

```
	if (m_attr.has(GraphAttributes::nodeLabel)) {
		std::vector<std::string> labels;
		std::size_t current, previous = 0;
		std::string str = m_attr.label(v);
			current = str.find('\n');
		while (current != std::string::npos) {
			labels.push_back(str.substr(previous, current - previous));
			previous = current + 1;
			current = str.find('\n', previous);
		}
		labels.push_back(str.substr(previous, current - previous));
		int i = -1;
		for (std::string& label_text: labels)
		{
			pugi::xml_node label = xmlNode.append_child("text");
			label.append_attribute("x") = m_attr.x(v);
			label.append_attribute("y") = m_attr.y(v) + (double)(i * 5);
			label.append_attribute("text-anchor") = "middle";
			label.append_attribute("dominant-baseline") = "middle";
			label.append_attribute("font-family") = m_settings.fontFamily().c_str();
			label.append_attribute("font-size") = 3;
			label.append_attribute("fill") = m_settings.fontColor().c_str();
			label.text() = label_text.c_str();

			if (m_attr.has(GraphAttributes::nodeLabelPosition)) {
				label.attribute("x") = m_attr.x(v) + m_attr.xLabel(v);
				label.attribute("y") = m_attr.y(v) + m_attr.yLabel(v);
			}
			i++;
		}
	}
```

This modification are required to draw the SVG with the coordinate in 3 lines (because there are 3 vertices) as SVG text does not support newline.
