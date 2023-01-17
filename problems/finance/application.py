import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application make "__name__" (= this .py file) a Flask server and save it's location in the variable app
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True


# Ensure responses aren't cached
#       Uses flask library defiinitons for chache handlers and overwites the default
#       cache handler wraper to behave the folowing way assigning different values to
#       certain flask parameters
#   MODIFIES THE AFTER REQUEST HANDLER
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

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    # Get portfolio
    portfolio = db.execute("SELECT symbol, name, SUM(shares) as total_shares FROM \
           all_transactions WHERE user_id = ? GROUP BY symbol", session["user_id"])

    # Get current prices of the stocks
    stock_prices = {}
    stock_totals = {}
    TOTAL = 0
    for stock in portfolio:
        quote = lookup(stock["symbol"])
        stock_prices[stock["symbol"]] = usd(quote["price"])
        stock_totals[stock["symbol"]] = usd(quote["price"] * stock["total_shares"])
        TOTAL += quote["price"] * stock["total_shares"]

    # Get cash avaliable
    cash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])[0]["cash"]
    TOTAL += cash

    # Format to USD
    cash = usd(cash)
    TOTAL = usd(TOTAL)

    return render_template("index.html", portfolio=portfolio, prices=stock_prices, totals=stock_totals, cash=cash, TOTAL=TOTAL)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":
        # Get input from user
        symbol = request.form.get("symbol")
        if not symbol:
            return apology("must provide stock symbol", 400)
        shares = request.form.get("shares")
        if not shares:
            return apology("must provide the amount of shares you whant to buy", 400)
        if not shares.isnumeric():
            return apology("must provide a numeric value for shares", 400)
        shares = int(shares)
        # Look for quote of symbol in IEX
        quote = lookup(symbol)

        # Ensure symbol is valid
        if quote is None:
            return apology("invalid symbol", 400)

        # Get share info
        symbol = quote["symbol"]
        name = quote["name"]
        price = quote["price"]
        total_price = price * shares

        # Get row for current user id session
        rows = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])
        # Ensure user id exist and theres just one user with that id
        if len(rows) != 1:
            return errorhandler(500)
        # Get cash
        cash = rows[0]["cash"]

        # Ensure there is enough cash to buy shares
        if cash < total_price:
            return apology("Not enough money to buy stocks")
        else:
            # Update users cash
            db.execute("UPDATE users SET cash = ? WHERE id = ?", (cash - total_price), session["user_id"])
            # Insert the transaction
            db.execute("INSERT INTO all_transactions (user_id, symbol, name, shares, price) \
                   VALUES(?, ?, ?, ?, ?)", session["user_id"], symbol, name, shares, price)
            # Redirect to index
            return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    histo = db.execute("SELECT symbol, shares, price, time FROM \
            all_transactions WHERE user_id = ? GROUP BY time", session["user_id"])
    print(histo)
    return render_template("history.html", history=histo)


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
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username").lower())

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
    """Get stock quote."""
    if request.method == "POST":
        # Get input from user
        symbol = request.form.get("symbol")
        # Look for quote of symbol in IEX
        quote = lookup(symbol)
        # Ensure symbol is valid
        if quote is None:
            return apology("invalid symbol", 400)
        # Show quote
        return render_template("quoted.html", name=quote["name"], price=usd(quote["price"]), sym=quote["symbol"])

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "POST":
        # Get from info
        user = request.form.get("username").lower()
        passw = request.form.get("password")
        passwcon = request.form.get("confirmation")

        # Ensure username was submitted
        if not user:
            return apology("must provide username", 400)

        # Ensure password was submitted
        elif not passw:
            return apology("must provide password", 400)

        # Ensure password was submitted
        elif not passwcon:
            return apology("must provide password confirmation", 400)

        # If all info was submited
        else:
            # Ensure username is unique
            rows = db.execute("SELECT * FROM users WHERE username = ?", user)
            if len(rows) == 1:
                return apology("the username already exists", 400)

            # Ensure pasword and pasword confirmation match
            if passw != passwcon:
                return apology("passwords do not match", 400)

            # Generate hash code for password
            hashed = generate_password_hash(passw)
            # Insert username and hash code to the db
            db.execute("INSERT INTO users (username, hash) VALUES(?, ?)", user, hashed)

            # Query database where username to get ID
            rows = db.execute("SELECT * FROM users WHERE username = ?", user)

            # Remember which user has logged in
            session["user_id"] = rows[0]["id"]

            # Redirect user to home page
            return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """ Sell shares of stock """
    if request.method == "POST":
        # Get user input
        toSell = request.form.get("shares")
        if not toSell:
            return apology("must provide the amount of shares you whant to sell", 400)
        toSell = int(toSell)
        symbol = request.form.get("symbol")

        # Get portfolio
        rows = db.execute("SELECT symbol FROM all_transactions WHERE user_id = ? GROUP BY symbol", session["user_id"])
        portfolio = []
        for item in rows:
            portfolio.append(item["symbol"])

        # Verify if symbol is on portfolio
        if not symbol in portfolio:
            return apology("You don't have this stock in your portfolio")
        else:
            # Get owned shares
            ownedShares = db.execute("SELECT SUM(shares) as total_shares FROM all_transactions \
                            WHERE user_id = ? AND symbol = ?", session["user_id"], symbol)[0]["total_shares"]

            # Verify there is enough shares to sell
            if ownedShares < toSell:
                return apology("You don't own that many shares")

            # Get row for current user id session
            rows = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])
            # Ensure user id exist and theres just one user with that id
            if len(rows) != 1:
                return errorhandler(500)
            # Get cash
            cash = rows[0]["cash"]

            # Quote the current price of the stock
            quote = lookup(symbol)

            # Get share info
            name = quote["name"]
            price = quote["price"]
            total_price = price * toSell

            # Update users cash
            db.execute("UPDATE users SET cash = ? WHERE id = ?", (cash + total_price), session["user_id"])
            # Insert the transaction
            db.execute("INSERT INTO all_transactions (user_id, symbol, name, shares, price) \
                   VALUES(?, ?, ?, ?, ?)", session["user_id"], symbol, name, - toSell, price)
            # Redirect to index
            return redirect("/")

    else:
        portfolio = db.execute("SELECT symbol FROM all_transactions WHERE user_id = ? GROUP BY symbol", session["user_id"])
        return render_template("sell.html", portfolio=portfolio)


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
