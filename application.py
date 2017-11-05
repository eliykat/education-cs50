from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure responses aren't cached
if app.config["DEBUG"]:
    @app.after_request
    def after_request(response):
        response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
        response.headers["Expires"] = 0
        response.headers["Pragma"] = "no-cache"
        return response

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    #Get name and number of stocks owned from db
    stocks = db.execute("SELECT symbol, SUM(quantity) FROM history WHERE user_id = :user_id GROUP BY symbol", user_id=session["user_id"])

    worth = 0

    #Get stock price from lookup()
    for stock in stocks:
        lookup_data = lookup(stock['symbol'])

        #Sanity check to ensure lookup() worked correctly
        if lookup_data == None:
            return apology("error getting stock info")

        #Save name of stock (this could be stored in our db, but if we are downloading data anyway we can just fetch it again)
        stock['name'] = lookup_data['name']

        #convert current price to USD format and save in new dict entry for current stock
        stock['price'] = usd(lookup_data['price'])

        #calculate total value of holding
        stock['total_value'] = usd(lookup_data['price'] * int(stock['SUM(quantity)']))

        #add to net worth
        worth += lookup_data['price'] * int(stock['SUM(quantity)'])

    #Get user's available cash
    cash = db.execute("SELECT cash FROM users WHERE id = :user_id", user_id=session["user_id"])

    #Add cash to worth
    worth += cash[0]['cash']

    return render_template("index.html", stocks=stocks, worth=usd(worth), cash=usd(cash[0]['cash']))


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    if request.method == "GET":
        return render_template("buy.html")

    elif request.method == "POST":

        if not request.form.get("symbol"):
            return apology("must enter stock")

        if not request.form.get("quantity"):
            return apology("must enter quantity")

        #lookup is case sensitive, so store symbol in a string so we have access to the upper() function
        symbol = request.form.get("symbol")

        #Get stock information from lookup()
        lookup_data = lookup(symbol.upper())

        #Check that stock exists
        if lookup_data == None:
            return apology("stock does not exist")

        #Check that quantity > 0
        quantity = int(request.form.get("quantity"))
        if quantity < 1:
            return apology ("must buy > 0 stocks")

        #Check that user has enough cash
        cash = db.execute("SELECT cash FROM users WHERE id = :user_id", user_id=session["user_id"])
        if cash[0]['cash'] < lookup_data['price'] * quantity:
            return apology("not enough cash")

        #Store details of purchase
        db.execute("INSERT INTO history (user_id, symbol, price, date, quantity) VALUES (:user_id, :symbol, :price, datetime(), :quantity)", user_id=session["user_id"], symbol=lookup_data['symbol'], price=lookup_data['price'], quantity=quantity)

        #Reduce cash
        db.execute("UPDATE users SET cash = :newcash WHERE id = :user_id", newcash=(cash[0]['cash'] - (lookup_data['price'] * quantity)), user_id=session["user_id"])

        return redirect("/")


@app.route("/changepwd", methods=["GET", "POST"])
@login_required
def changepwd():

    #If user reached the route via GET, present them the page to change password
    if request.method == "GET":
        return render_template("changepwd.html")

    #If user reached the route via POST, they have submitted a password change request
    elif request.method == "POST":

        # Ensure password was submitted
        if not request.form.get("password"):
            return apology("must provide password", 403)

        # Ensure new password was submitted
        if not request.form.get("newpassword"):
            return apology("must provide new password", 403)

        #Check passwords match
        if request.form.get("newpassword") != request.form.get("newpassword-confirm"):
            return apology("new passwords do not match", 403)

        # Query database for user details
        user = db.execute("SELECT * FROM users WHERE id = :user_id",
                          user_id=session["user_id"])

        #Check current password is correct
        if not check_password_hash(user[0]['hash'], request.form.get("password")):
            return apology("incorrect password", 403)

        #Enter user information into database
        user_id = db.execute("UPDATE users SET hash = :password WHERE id = :user_id",
                    password=generate_password_hash(request.form.get("newpassword")), user_id=session["user_id"])

        return redirect("/")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    #Get info from db
    stocks = db.execute("SELECT * FROM history WHERE user_id = :user_id ORDER BY date", user_id=session["user_id"])

    for stock in stocks:
        stock['price'] = usd(stock['price'])

    return render_template("history.html", stocks=stocks)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    if request.method == "GET":
        return render_template("quote.html")

    elif request.method == "POST":

        #lookup is case sensitive, so store symbol in a string so we have access to the upper() function
        symbol = request.form.get("symbol")

        #Get stock information from lookup()
        lookup_data = lookup(symbol.upper())

        #Check that stock exists
        if lookup_data == None:
            return apology("stock does not exist")

        #Convert price into USD format
        lookup_data['price'] = usd(lookup_data['price'])

        return render_template("quoted.html", stock=lookup_data)


@app.route("/register", methods=["GET", "POST"])
def register():

    #If user reached the route via GET, present them the page to register
    if request.method == "GET":
        return render_template("register.html")

    #If user reached the route via POST, they have submitted a username and password for registration
    elif request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        #Check passwords match
        if request.form.get("password") != request.form.get("password-confirm"):
            return apology("passwords do not match", 403)

        # Query database for username to ensure it is not already taken
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))
        if len(rows) > 0:
            return apology("username is not available", 403)

        #Enter user information into database
        user_id = db.execute("INSERT INTO users (username, hash) VALUES (:username, :password)",
                    username=request.form.get("username"), password=generate_password_hash(request.form.get("password")))

        #Store the user's id in the session variable to effectively log us in
        #TODO : MAKE THIS WORK
        session["user_id"] = user_id

        return redirect("/")

@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    if request.method == "GET":
        return render_template("sell.html")

    elif request.method == "POST":

        if not request.form.get("symbol"):
            return apology("must enter stock")

        if not request.form.get("quantity"):
            return apology("must enter quantity")

        #lookup is case sensitive, so store symbol in a string so we have access to the upper() function
        symbol = request.form.get("symbol")
        symbol = symbol.upper()

        #Get stock information from lookup()
        lookup_data = lookup(symbol)

        #Check that stock exists
        if lookup_data == None:
            return apology("stock does not exist")

        #Check that quantity > 0
        quantity = int(request.form.get("quantity"))
        if quantity < 1:
            return apology ("must sell > 0 stocks")

        #Check that user has enough stocks
        stocks = db.execute("SELECT symbol, SUM(quantity) FROM history WHERE user_id = :user_id AND symbol = :symbol", user_id=session["user_id"], symbol=symbol)
        if len(stocks) < 1:
            return apology("you don't own that stock")
        if stocks[0]['SUM(quantity)'] < quantity:
            return apology("not enough stocks")

        #Store transaction
        db.execute("INSERT INTO history (user_id, symbol, price, date, quantity) VALUES (:user_id, :symbol, :price, datetime(), :quantity)", user_id=session["user_id"], symbol=lookup_data['symbol'], price=lookup_data['price'], quantity=quantity * -1)

        #Increase cash
        cash = db.execute("SELECT cash FROM users WHERE id = :user_id", user_id=session["user_id"])
        db.execute("UPDATE users SET cash = :newcash WHERE id = :user_id", newcash=(cash[0]['cash'] + (lookup_data['price'] * quantity)), user_id=session["user_id"])

        return redirect("/")


def errorhandler(e):
    """Handle error"""
    return apology(e.name, e.code)


# listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
