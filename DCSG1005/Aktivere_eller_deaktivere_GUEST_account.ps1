# ============================================
# Skript:    Aktivere_eller_deaktivere_GUEST_account.ps1
# Formål:    Aktivere eller deaktivere Guest-kontoen i Active Directory
# Forfatter: Kristina Jonsen
# Dato:      07.03.2026
# ============================================
<#
.SYNOPSIS
Aktiverer eller deaktiverer Guest-kontoen i Active Directory basert på valgt handling.

.DESCRIPTION
Skriptet tar inn en parameter som angir om Guest-kontoen skal aktiveres eller deaktiveres.
Det ber om domene-administrator-legitimasjon, henter Guest-brukeren fra domenet og
kjører riktig AD-kommando for å endre kontostatus.

.PARAMETER Action
Angir hvilken handling som skal utføres: Enable eller Disable.

.PARAMETER Server
Angir hvilket AD-domene/server som skal brukes. Standard er domenet brukeren er logget inn i.

.EXAMPLE
.\VALGT-AKTIVERE_ELLER_DEAKTIVERE_GUEST_ACCOUNT.ps1 -Action Enable
Aktiverer Guest-kontoen i domenet og ber om domene-admin-bruker og passord.

.EXAMPLE
.\VALGT-AKTIVERE_ELLER_DEAKTIVERE_GUEST_ACCOUNT.ps1 -Action Disable -WhatIf
Viser hva som ville skjedd hvis Guest-kontoen ble deaktivert, uten å endre noe.

.NOTES
Krever ActiveDirectory-modulen og at skriptet kjøres som administrator,
og at oppgitt domene-konto har nødvendige rettigheter i domenet.

#>

Import-Module ActiveDirectory

param(
    [Parameter(Mandatory)]
    [ValidateSet("Enable", "Disable")]
    [string]$Action,
    [string]$Server = "sec.core" 
)

# Ber bruker oppgi legitimasjon
$ADCredential = Get-Credential -Message "Skriv inn domene-admin for å Aktivere/Deaktivere Guest"

# Henter Guest-kontoen i domenet
$Guest = Get-ADUser -Identity "Guest" -Server $Server -Credential $ADCredential -ErrorAction Stop

# Aktiverer eller deaktiverer Guest utifra status med en switch
switch ($Action) {
    'Enable' {
        Enable-ADAccount -Identity $Guest -Server $Server -Credential $ADCredential
        Write-Output "Guest-kontoen er aktivert."
    }
    'Disable' {
        Disable-ADAccount -Identity $Guest -Server $Server -Credential $ADCredential
        Write-Output "Guest-kontoen er deaktivert."
    }
}
