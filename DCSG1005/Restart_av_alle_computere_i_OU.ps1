# ============================================
# Skript: Restart_av_alle_computere_i_OU.ps1
# Formål: Restarte aktive datamaskiner i valgt OU, unntatt ekskluderte maskiner
# Krav: Må kjøres som administator i Powershell 7
# Forfatter: Kristina Jonsen
# Dato: 2026-03-04
# ============================================
<#
.SYNOPSIS
Restarter aktive datamaskiner i angitt søkebase i Active Directory.

.DESCRIPTION
Skriptet henter alle datamaskinkontoer som er aktivert i Active Directory
innenfor søkebasen definert i $SearchBase, inkludert underliggende objekter.
Deretter filtreres maskiner i ekskluderingslisten bort, før restart-kommando
sendes til de resterende maskinene

Skriptet ber om legitimering ved kjøring for å restarte markinene.

.PARAMETER None
Skriptet har ingen paramtere. OU velges interaktiv.

.EXAMPLE
.\VALGT-RESTART_AV_ALLE_COMPUTERE_I_OU.ps1

Henter alle aktiverte datamaskiner fra definert søkebase i Active Directory
og restarter dem, unntatt maskiner som er lagt inn i ekskluderingslisten.

.NOTES
Krever ActiveDirectory-modulen og tilgang til å bruke Restart-Computer mot
de aktuelle maskinene.

Må kjøres som administrator.
#>

Import-Module ActiveDirectory

# Setter hvilken OU som skal søkes i
$SearchBase = "CN=Computers,DC=sec,DC=core" # set manual path

# Liste over maskiner som ikke skal restartes
$Excluded   = @("[Main_computer_running_the_script]")  

# Ber bruker oppgi legitimasjon som brukes ved restart av markiner
$credential = Get-Credential

# Henter alle aktive (enabled) datamaskinkontoer fra AD i angitt søkebase
Get-ADComputer -SearchBase $SearchBase -SearchScope Subtree -Filter 'Enabled -eq $true' |
    # Filtrerer bort maskiner som står i ekskluderingslisten
    Where-Object { $_.Name -notin $Excluded } |
    ForEach-Object { 
        Restart-Computer -ComputerName $_.Name -Credential $credential -Force
    }
