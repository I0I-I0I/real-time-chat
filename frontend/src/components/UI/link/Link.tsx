import cls from "@/utils/cls"
import React from "react"
import styles from "./Link.module.css"
import { NavLink } from "react-router-dom"

type LinkVariants = "default"

interface LinkProps extends React.AnchorHTMLAttributes<HTMLAnchorElement> {
    href: string
    children: React.ReactNode
    className?: string
    variant?: LinkVariants
}

export const Link = ({
    children,
    className = "",
    variant = "default",
    href,
    ...props
}: LinkProps) => (
    <NavLink className={cls(styles.link, styles[variant], className)} to={href} {...props}>
        {children}
    </NavLink>
)
