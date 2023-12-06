from django.shortcuts import render
from . models import Message

# Create your views here.
def homepage(request):

    if request.method == "POST":
        name = request.POST['user_name']
        email = request.POST['emailaddress']
        phonenumber = request.POST['phonenumber']

        new_message = Message(name=name, email=email, phone=phonenumber)
        new_message.save()

    #print("Test")
    return render(request, 'home.html')