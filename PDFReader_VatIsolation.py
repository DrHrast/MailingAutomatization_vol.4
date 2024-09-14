import PyPDF2

def oib_isolation(location_folder:str, file_name:str):
    # Construct full file path
    full_fName = location_folder + "\\" + file_name

    try:
        # Open PDF file in read-binary mode
        file = open(full_fName, 'rb')
        # Initialize PdfFileReader object
        fileReader = PyPDF2.PdfReader(file)
        # Get the first page
        filePage = fileReader.pages[0]
        # Extract text from the page
        tekst = filePage.extract_text()

        # You can adjust the range according to where the VAT is located
        tekst2 = tekst[480:550]

        # Initialize empty VAT string and counter
        vat = ''
        counter = 0

        # Iterate through the text and capture digits for VAT (11 digits)
        for i in range(len(tekst2)):
            if tekst2[i].isdigit() and counter < 11:
                vat += tekst2[i]
                counter += 1

        # Close the file
        file.close()

        # Return the extracted VAT number
        return vat

    except Exception as e:
        # Handle any exceptions that may occur
        print(f"Error while reading the PDF file: {str(e)}")
        return ""
